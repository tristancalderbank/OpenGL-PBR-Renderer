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

    mBloomFramebuffers[0] = std::make_unique<BloomFramebuffer>(mEngineConfig.initialViewportWidth, mEngineConfig.initialViewportHeight);
    mBloomFramebuffers[0]->init();
    mBloomFramebuffers[1] = std::make_unique<BloomFramebuffer>(mEngineConfig.initialViewportWidth, mEngineConfig.initialViewportHeight);
    mBloomFramebuffers[1]->init();

    // Shader
    mPbrShader = std::make_unique<Shader>(mEngineConfig.engineRootPath + "/src/shaders/pbr.vert", mEngineConfig.engineRootPath + "/src/shaders/pbr.frag");
    mBloomShader = std::make_unique<Shader>(mEngineConfig.engineRootPath + "/src/shaders/bloom.vert", mEngineConfig.engineRootPath + "/src/shaders/bloom.frag");
    mPostShader = std::make_unique<Shader>(mEngineConfig.engineRootPath + "/src/shaders/post.vert", mEngineConfig.engineRootPath + "/src/shaders/post.frag");
    mSkyboxShader = std::make_unique<Shader>(mEngineConfig.engineRootPath + "/src/shaders/skybox.vert", mEngineConfig.engineRootPath + "/src/shaders/skybox.frag");

    // Pre-compute IBL stuff
    mIblEquirectangularCubemap = std::make_unique<EquirectangularCubemap>(mEngineConfig.engineRootPath, mEngineConfig.hdriPath);
    mIblEquirectangularCubemap->compute();

    mIblDiffuseIrradianceMap = std::make_unique<DiffuseIrradianceMap>(mEngineConfig.engineRootPath, mIblEquirectangularCubemap->getCubemapId());
    mIblDiffuseIrradianceMap->compute();

    mIblSpecularMap = std::make_unique<SpecularMap>(mEngineConfig.engineRootPath, mIblEquirectangularCubemap->getCubemapId());
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
        mBloomFramebuffers[0]->resize(windowSize.width, windowSize.height);
        mBloomFramebuffers[1]->resize(windowSize.width, windowSize.height);
    }

    ImGui::CollapsingHeader("General");
    ImGui::Text("Average FPS %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    mCameraManager->drawDebugPanel();

    ImGui::CollapsingHeader("Post-processing");

    ImGui::Text("Bloom");
    ImGui::Checkbox("Enabled", &mBloomEnabled);
    ImGui::SliderFloat("Intensity", &mBloomIntensity, 0.0, 5.0);
    ImGui::SliderFloat("Threshold", &mBloomBrightnessCutoff, 0.01, 5.0);
    ImGui::SliderInt("Blur Iterations", &mBloomIterations, 2, 20);

    ImGui::Text("Post");
    ImGui::Checkbox("HDR Tone Mapping (Reinhard)", &mTonemappingEnabled);
    ImGui::SliderFloat("Gamma Correction", &mGammaCorrectionFactor, 1.0, 3.0);

    // Rendering
    // Main pass
    mFramebuffer->bind();
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
    mSkyboxShader->setFloat("bloomBrightnessCutoff", mBloomBrightnessCutoff);
    mSkybox->Draw();

    // Bloom pass
    glm::vec2 blurDirectionX = glm::vec2(1.0f, 0.0f);
    glm::vec2 blurDirectionY = glm::vec2(0.0f, 1.0f);

    mBloomFramebuffers[0]->bind();
    mBloomShader->use();
    // first iteration we use the bloom buffer from the main render pass
    glBindTexture(GL_TEXTURE_2D, mFramebuffer->getBloomColorTextureId());
    glGenerateMipmap(GL_TEXTURE_2D);
    mBloomShader->setInt("sampleMipLevel", 5);
    mBloomShader->setVec2("blurDirection", blurDirectionX);
    mFullscreenQuad->Draw();
    mBloomShader->setInt("sampleMipLevel", 0);

    unsigned int bloomFramebuffer = 1; // which buffer to use

    for (auto i = 1; i < mBloomIterations; i++) {
        unsigned int sourceBuffer = bloomFramebuffer == 1 ? 0 : 1;
        mBloomFramebuffers[bloomFramebuffer]->bind();
        auto blurDirection = bloomFramebuffer == 1 ? blurDirectionY : blurDirectionX;
        mBloomShader->setVec2("blurDirection", blurDirection);
        glBindTexture(GL_TEXTURE_2D, mBloomFramebuffers[sourceBuffer]->getColorTextureId());
        mFullscreenQuad->Draw();
        bloomFramebuffer = sourceBuffer;
    }

    // Post-processing pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // switch back to default fb
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mPostShader->use();

    mPostShader->setBool("bloomEnabled", mBloomEnabled);
    mPostShader->setFloat("bloomIntensity", mBloomIntensity);
    mPostShader->setBool("tonemappingEnabled", mTonemappingEnabled);
    mPostShader->setFloat("gammaCorrectionFactor", mGammaCorrectionFactor);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mBloomFramebuffers[bloomFramebuffer]->getColorTextureId());
    mPostShader->setInt("colorTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mBloomFramebuffers[bloomFramebuffer]->getColorTextureId());
    mPostShader->setInt("bloomTexture", 1);

    mFullscreenQuad->Draw();

    // draw ImGui
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // check events and swap back buffer to display it in the window
    glfwSwapBuffers(mWindowManager->getWindow());
}