#pragma once

#include "inputmanager.h"

#include <iostream>

#include "glfw3.h"
#include <imgui/imgui.h>

#include "engineconfig.h"

InputManager::InputManager(EngineConfig &engineConfig, std::shared_ptr<WindowManager> windowManager) : mEngineConfig(engineConfig), mWindowManager(windowManager) {}

void InputManager::startup() {}

void InputManager::update() {
    mCurrentActions.clear(); // clear any actions from last frame

    glfwPollEvents();
    glfwGetCursorPos(mWindowManager->getWindow(), &mMousePosition.x, &mMousePosition.y);

    for(auto iter = mKeymap.begin(); iter != mKeymap.end(); iter++) {
        int key = iter->first;

        if (glfwGetKey(mWindowManager->getWindow(), key) == GLFW_PRESS) {
            mCurrentActions.insert(iter->second);
        }
    }
}

void
InputManager::enableGuiMouse() {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = io.ConfigFlags & !ImGuiConfigFlags_NoMouse;
};

void
InputManager::disableGuiMouse() {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = io.ConfigFlags | ImGuiConfigFlags_NoMouse;
}

void
InputManager::showMouseCursor() {
    glfwSetInputMode(mWindowManager->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
};

void
InputManager::hideMouseCursor() {
    glfwSetInputMode(mWindowManager->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
};

const std::set<KeymapAction>&
InputManager::getCurrentActions() {
   return mCurrentActions;
}

const MousePosition&
InputManager::getMousePosition() {
    return mMousePosition;
}
