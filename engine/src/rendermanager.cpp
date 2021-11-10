#include "rendermanager.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "stb_image.h"

#include "camera.h"

RenderManager::RenderManager(EngineConfig &engineConfig, std::shared_ptr<WindowManager> windowManager, std::shared_ptr<CameraManager> cameraManager) : mEngineConfig(engineConfig), mWindowManager(windowManager), mCameraManager(cameraManager) {

}

void RenderManager::startup(std::shared_ptr<Scene> scene)
{
    mScene = scene;

    // GLAD manages the function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // interpolate between cubemap faces

    // GLFW options

    // stb options
    stbi_set_flip_vertically_on_load(true);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.FontGlobalScale = mEngineConfig.imguiFontScale;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(mWindowManager->getWindow(), true);
    ImGui_ImplOpenGL3_Init(mEngineConfig.glslVersion);

    // Init any intermediate framebuffers
    mFramebuffer = std::make_unique<Framebuffer>(mEngineConfig.initialViewportWidth, mEngineConfig.initialViewportHeight);
    mFramebuffer->init();

    // Shader
    mPbrShader = std::make_unique<Shader>("shaders/pbr.vert", "shaders/pbr.frag");
    mPostShader = std::make_unique<Shader>("shaders/post.vert", "shaders/post.frag");
    mSkyboxShader = std::make_unique<Shader>("shaders/skybox.vert", "shaders/skybox.frag");

    // Pre-compute IBL stuff
    mIblEquirectangularCubemap = std::make_unique<EquirectangularCubemap>("../engine", mEngineConfig.hdriPath);
    mIblEquirectangularCubemap->compute();

    mIblDiffuseIrradianceMap = std::make_unique<DiffuseIrradianceMap>("../engine", mIblEquirectangularCubemap->getCubemapId());
    mIblDiffuseIrradianceMap->compute();

    mIblSpecularMap = std::make_unique<SpecularMap>("../engine", mIblEquirectangularCubemap->getCubemapId());
    mIblSpecularMap->computePrefilteredEnvMap();
    mIblSpecularMap->computeBrdfConvolutionMap();

    glViewport(0, 0, mEngineConfig.initialViewportWidth, mEngineConfig.initialViewportHeight); // set initial viewport size

    // Models/Geometry
    mFullscreenQuad = std::make_unique<FullscreenQuad>();
    mSkybox = std::make_unique<Skybox>(mIblEquirectangularCubemap->getCubemapId());
}

void RenderManager::shutdown()
{

}

void RenderManager::startGuiFrame() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("NanoEngine");
}

void RenderManager::render()
{
    if (mWindowManager->windowResized()) {
        auto windowSize = mWindowManager->getWindowSize();
        glViewport(0, 0, windowSize.width, windowSize.height);
        mFramebuffer->resize(windowSize.width, windowSize.height);
    }

    ImGui::CollapsingHeader("General");
    ImGui::Text("Average FPS %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::CollapsingHeader("Post-processing");
    ImGui::SliderFloat("Bloom Brightness Cutoff", &mBloomBrightnessCutoff, 0.01, 5.0);
    ImGui::Checkbox("HDR Tone Mapping (Reinhard)", &mTonemappingEnabled);
    ImGui::SliderFloat("Gamma Correction", &mGammaCorrectionFactor, 1.0, 3.0);

    // Rendering
    // Main pass
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer->getFramebufferId());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 cameraPosition = mCameraManager->getPosition();
    glm::mat4 projection = mCameraManager->getProjectionMatrix();
    glm::mat4 view = mCameraManager->getViewMatrix();

    mPbrShader->use();

    mPbrShader->setVec3Array("lightPositions", mScene->mLightPositions);
    mPbrShader->setVec3Array("lightColors", mScene->mLightColors);
    mPbrShader->setVec3("cameraPosition", cameraPosition);

    // IBL stuff
    glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
    mPbrShader->setInt("diffuseIrradianceMap", TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mIblDiffuseIrradianceMap->getCubemapId());

    glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_PREFILTERED_ENV_MAP);
    mPbrShader->setInt("prefilteredEnvMap", TEXTURE_UNIT_PREFILTERED_ENV_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mIblSpecularMap->getPrefilteredEnvMapId());

    glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
    mPbrShader->setInt("brdfConvolutionMap", TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
    glBindTexture(GL_TEXTURE_2D, mIblSpecularMap->getBrdfConvolutionMapId());

    // post stuff for main shader
    mPbrShader->setFloat("bloomBrightnessCutoff", mBloomBrightnessCutoff);

    for (auto entity : mScene->mEntities) {
        glm::mat4 model = glm::mat4(1.0f);

        auto rotationMatrix = glm::toMat4(entity.getOrientation());
        model = rotationMatrix * model;

        model = glm::translate(model, entity.getPosition());
        model = glm::scale(model, entity.getScale());

        mPbrShader->setModelViewProjectionMatrices(model, view, projection);

        entity.getModel()->Draw(*mPbrShader);
    }

    // skybox (draw this last to avoid running fragment shader in places where objects are present
    mSkyboxShader->use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // remove translation so skybox is always surrounding camera

    mSkyboxShader->setModelViewProjectionMatrices(model, skyboxView, projection);
    mSkyboxShader->setInt("skybox", 0); // set skybox sampler to slot 0
    mSkybox->Draw();

    // Post-processing pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // switch back to default fb
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mPostShader->use();

    mPostShader->setBool("tonemappingEnabled", mTonemappingEnabled);
    mPostShader->setFloat("gammaCorrectionFactor", mGammaCorrectionFactor);

    glActiveTexture(GL_TEXTURE0);
    mPostShader->setInt("colorTexture", 0);
    glBindTexture(GL_TEXTURE_2D, mFramebuffer->getBloomColorTextureId());
    mFullscreenQuad->Draw();

    // draw ImGui
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // check events and swap back buffer to display it in the window
    glfwSwapBuffers(mWindowManager->getWindow());
}