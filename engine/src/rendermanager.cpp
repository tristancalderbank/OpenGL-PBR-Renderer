#include "rendermanager.h"

#include "glm/glm.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "stb_image.h"

#include "camera.h"
#include "framebuffer.h"
#include "model.h"

// viewport
int INITIAL_VIEWPORT_WIDTH = 800;
int INITIAL_VIEWPORT_HEIGHT = 600;
const float IMGUI_FONT_SCALE = 1.0f;

// camera
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraInitialPosition = glm::vec3(0.0f, 0.0f, 3.0f);
float cameraInitialYaw = 0;
float cameraInitialPitch = 0;
Camera camera(
    cameraUp,
    cameraInitialPosition,
    cameraInitialYaw,
    cameraInitialPitch,
    INITIAL_VIEWPORT_WIDTH,
    INITIAL_VIEWPORT_HEIGHT
);

// time
float frameTimeDelta = 0.0f; // time between current frame and last frame
float lastFrameTime = 0.0f; // time of last frame

// Intermediate buffers
Framebuffer framebuffer(INITIAL_VIEWPORT_WIDTH, INITIAL_VIEWPORT_HEIGHT);

// invoked on window resizes, update gl window to the current size
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    framebuffer.resize(width, height);
    camera.setWindowDimensions(width, height);
}

bool mouseCameraEnabled = true;

// handle input such as keyboard
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        mouseCameraEnabled = true;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        mouseCameraEnabled = false;
    }
}

void processMouseMovement(GLFWwindow* window, double xPos, double yPos) {
    if (mouseCameraEnabled) {
        camera.processMouseMovement(window, xPos, yPos);
    }
}

void RenderManager::startup()
{
    glfwInit();

    // see window creation options here: https://www.glfw.org/docs/latest/window.html#window_hints
    const char* glslVersion = "#version 330";
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core profile doesn't include unneeded backwards compat features

    mWindow = glfwCreateWindow(INITIAL_VIEWPORT_WIDTH, INITIAL_VIEWPORT_HEIGHT, "NanoEngine", NULL, NULL);

    if (mWindow == NULL) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(mWindow);

    // GLAD manages the function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // interpolate between cubemap faces

    // GLFW options
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
    glfwSetCursorPosCallback(mWindow, processMouseMovement);
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // stb options
    stbi_set_flip_vertically_on_load(true);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.FontGlobalScale = IMGUI_FONT_SCALE;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    // Init any intermediate framebuffers
    framebuffer.init();

    // Shader
    mPbrShader = std::make_unique<Shader>("shaders/pbr.vert", "shaders/pbr.frag");
    mPostShader = std::make_unique<Shader>("shaders/post.vert", "shaders/post.frag");
    mSkyboxShader = std::make_unique<Shader>("shaders/skybox.vert", "shaders/skybox.frag");

    // Pre-compute IBL stuff
    mIblEquirectangularCubemap = std::make_unique<EquirectangularCubemap>("../engine", "resources/hdr/barcelona_rooftop.hdr");
    mIblEquirectangularCubemap->compute();

    mIblDiffuseIrradianceMap = std::make_unique<DiffuseIrradianceMap>("../engine", mIblEquirectangularCubemap->getCubemapId());
    mIblDiffuseIrradianceMap->compute();

    mIblSpecularMap = std::make_unique<SpecularMap>("../engine", mIblEquirectangularCubemap->getCubemapId());
    mIblSpecularMap->computePrefilteredEnvMap();
    mIblSpecularMap->computeBrdfConvolutionMap();

    glViewport(0, 0, INITIAL_VIEWPORT_WIDTH, INITIAL_VIEWPORT_HEIGHT); // set initial viewport size

    // Models/Geometry
    mFullscreenQuad = std::make_unique<FullscreenQuad>();
    mSkybox = std::make_unique<Skybox>(mIblEquirectangularCubemap->getCubemapId());

    stbi_set_flip_vertically_on_load(false);
    mHelmet = std::make_unique<Model>("resources/helmet/DamagedHelmet.gltf");
    stbi_set_flip_vertically_on_load(true);
}

void RenderManager::shutdown()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void RenderManager::render()
{
    while (!glfwWindowShouldClose(mWindow)) {
        // calculate frame time
        float currentTime = glfwGetTime();
        frameTimeDelta = lastFrameTime - currentTime;
        lastFrameTime = currentTime;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("NanoEngine");                          // Create a window called "Hello, world!" and append into it.
        ImGui::CollapsingHeader("General");
        ImGui::Text("Average FPS %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        camera.drawDebugPanel();

        ImGui::CollapsingHeader("Model");
        float scale = 1.0;
        ImGui::SliderFloat("scale", &scale, 0.0, 100);

        ImGui::CollapsingHeader("Post-processing");
        ImGui::Checkbox("HDR Tone Mapping (Reinhard)", &mTonemappingEnabled);
        ImGui::SliderFloat("Gamma Correction", &mGammaCorrectionFactor, 1.0, 3.0);

        ImGui::End();

        // ImGui::ShowDemoWindow();

        ImGuiIO& io = ImGui::GetIO();

        if (mouseCameraEnabled) {
            io.ConfigFlags = io.ConfigFlags | ImGuiConfigFlags_NoMouse;
        }
        else {
            io.ConfigFlags = io.ConfigFlags & !ImGuiConfigFlags_NoMouse;
        }

        // input
        processInput(mWindow);
        glfwSetInputMode(mWindow, GLFW_CURSOR, mouseCameraEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); // don't show cursor on the windo
        camera.processKeyboard(mWindow, frameTimeDelta);
        glm::vec3 cameraPosition = camera.getPosition();

        // Rendering
        // Main pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.getFramebufferHandle());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model;

        // sphere (PBR)
        mPbrShader->use();
        model = glm::mat4(1.0f);
        model = glm::rotate(model, 1.5708f, glm::vec3(1.0f, 0.0f, 0.0f)); // where x, y, z is axis of rotation (e.g. 0 1 0)
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        mPbrShader->setModelViewProjectionMatrices(model, view, projection);

        mPbrShader->setVec3Array("lightPositions", mLightPositions);
        mPbrShader->setVec3Array("lightColors", mLightColors);
        mPbrShader->setVec3("cameraPosition", cameraPosition);

        glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
        mPbrShader->setInt("diffuseIrradianceMap", TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mIblDiffuseIrradianceMap->getCubemapId());

        glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_PREFILTERED_ENV_MAP);
        mPbrShader->setInt("prefilteredEnvMap", TEXTURE_UNIT_PREFILTERED_ENV_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mIblSpecularMap->getPrefilteredEnvMapId());

        glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
        mPbrShader->setInt("brdfConvolutionMap", TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
        glBindTexture(GL_TEXTURE_2D, mIblSpecularMap->getBrdfConvolutionMapId());

        mHelmet->Draw(*mPbrShader);

        // skybox (draw this last to avoid running fragment shader in places where objects are present
        mSkyboxShader->use();
        model = glm::mat4(1.0f);
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
        glBindTexture(GL_TEXTURE_2D, framebuffer.getColorTextureHandle());
        mFullscreenQuad->Draw();

        // draw ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // check events and swap back buffer to display it in the window
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}