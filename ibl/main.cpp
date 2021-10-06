#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stb_image.h>

#include "camera.h"
#include "framebuffer.h"
#include "fullscreenquad.h"
#include "hdrtexture.h"
#include "model.h"
#include "shader.h"
#include "skybox.h"

#include "hdricube.h"

// shaders
std::string postVertexShaderPath = "shaders/post.vert";
std::string postFragmentShaderPath = "shaders/post.frag";

std::string skyboxVertexShaderPath = "shaders/skybox.vert";
std::string skyboxFragmentShaderPath = "shaders/skybox.frag";

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

/**
 *
 * @param argc number of args
 * @param argv name, viewport_width, viewport_height
 * @return
 */
int main(int argc, const char * argv[])
{
    glfwInit();

    // see window creation options here: https://www.glfw.org/docs/latest/window.html#window_hints
    const char* glslVersion = "#version 330";
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core profile doesn't include unneeded backwards compat features

    GLFWwindow* window = glfwCreateWindow(INITIAL_VIEWPORT_WIDTH, INITIAL_VIEWPORT_HEIGHT, "TinyEngine", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD manages the function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // OpenGL options

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, INITIAL_VIEWPORT_WIDTH, INITIAL_VIEWPORT_HEIGHT); // set initial viewport size

    // GLFW options
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, processMouseMovement);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // stb options
    stbi_set_flip_vertically_on_load(true);

    // Init any intermediate framebuffers
    framebuffer.init();

    // Shader
    Shader postShader(postVertexShaderPath.c_str(), postFragmentShaderPath.c_str());
    Shader skyboxShader(skyboxVertexShaderPath.c_str(), skyboxFragmentShaderPath.c_str());
    Shader hdriShader("shaders/hdricube.vert", "shaders/hdricube.frag");

    // Model
    FullscreenQuad fullscreenQuad;
    Skybox skybox("resources/skybox");
    Model sphere("resources/sphere/sphere.gltf");

    // Lights
    std::vector<glm::vec3> lightPositions = {
        glm::vec3(0.0f, 0.0f, 10.0f)
    };

    std::vector<glm::vec3> lightColors = {
        glm::vec3(150.0f, 150.0f, 150.0f)
    };

    // start IBL stuff
    HDRICube hdriCube = HDRICube("resources/hdr/newport_loft.hdr");

    // end IBL stuff

    while (!glfwWindowShouldClose(window)) {
        // calculate frame time
        float currentTime = glfwGetTime();
        frameTimeDelta = lastFrameTime - currentTime;
        lastFrameTime = currentTime;

        // input
        processInput(window);
        glfwSetInputMode(window, GLFW_CURSOR, mouseCameraEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); // don't show cursor on the windo
        camera.processKeyboard(window, frameTimeDelta);
        glm::vec3 cameraPosition = camera.getPosition();

        // Rendering
        // Main pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.getFramebufferHandle());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model;

        model = glm::mat4(1.0f);

        // cube
        hdriShader.use();
        hdriShader.setModelViewProjectionMatrices(model, view, projection);
        hdriCube.Draw(hdriShader);

        // Post-processing pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // switch back to default fb
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        postShader.use();
        fullscreenQuad.Draw(postShader, framebuffer.getColorTextureHandle());

        // check events and swap back buffer to display it in the window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}