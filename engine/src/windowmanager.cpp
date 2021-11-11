#include "windowmanager.h"

#include <iostream>

#include "glfw3.h"

#include "engineconfig.h"

WindowManager::WindowManager(EngineConfig &engineConfig) : mEngineConfig(engineConfig) {
    mWindowSize.width = engineConfig.initialViewportWidth;
    mWindowSize.height = engineConfig.initialViewportHeight;
}

void WindowManager::startup(std::shared_ptr<InputManager> inputManager) {
    mInputManager = inputManager;
    glfwInit();

    // see window creation options here: https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core profile doesn't include unneeded backwards compat features

    mWindow = glfwCreateWindow(mEngineConfig.initialViewportWidth, mEngineConfig.initialViewportHeight, "NanoEngine", NULL, NULL);

    if (mWindow == NULL) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(mWindow);
}

void
WindowManager::update() {
    auto actions = mInputManager->getCurrentActions();

    auto iter = actions.find(KeymapAction::EXIT);
    if (iter != actions.end()) {
        glfwSetWindowShouldClose(mWindow, true);
    }

    WindowSize windowSize;

    glfwGetFramebufferSize(mWindow, &windowSize.width, &windowSize.height);

    mWindowResized = windowSize.width != mWindowSize.width || windowSize.height != mWindowSize.height;

    if (mWindowResized) {
        mWindowSize = windowSize;
    }
}

void
WindowManager::clearResizeFlag() {
    mWindowResized = false;
}

void
WindowManager::shutdown() {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

GLFWwindow*
WindowManager::getWindow() {
    return mWindow;
}

bool
WindowManager::windowShouldClose() {
    return glfwWindowShouldClose(mWindow);
}

bool
WindowManager::windowResized() {
    return mWindowResized;
}

const WindowSize &WindowManager::getWindowSize() {
    return mWindowSize;
}

