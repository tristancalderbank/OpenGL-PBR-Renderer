#include "cameramanager.h"

#include "camera.h"

CameraManager::CameraManager(EngineConfig &engineConfig, std::shared_ptr<WindowManager> windowManager, std::shared_ptr<InputManager> inputManager) : mEngineConfig(engineConfig), mWindowManager(windowManager), mInputManager(inputManager) {}

void CameraManager::startup() {
    // camera
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraInitialPosition = glm::vec3(0.0f, 0.0f, 3.0f);

    float cameraInitialYaw = 0;
    float cameraInitialPitch = 0;

    mCamera = std::make_unique<Camera>(
            cameraUp,
            cameraInitialPosition,
            cameraInitialYaw,
            cameraInitialPitch,
            mEngineConfig.initialViewportWidth,
            mEngineConfig.initialViewportHeight
    );

    mInputManager->hideMouseCursor();
    mInputManager->disableGuiMouse();
}

void CameraManager::update(float frameTimeDelta) {
    // pass actions to camera
    if (mWindowManager->windowResized()) {
        mCamera->setWindowDimensions(mWindowManager->getWindowSize());
    }

    auto actions = mInputManager->getCurrentActions();

    if (actions.find(KeymapAction::DISABLE_MOUSE_CAMERA) != actions.end()) {
        mouseCameraEnabled = false;
        mInputManager->showMouseCursor();
        mInputManager->enableGuiMouse();
        mCamera->resetMouse();
    }

    if (actions.find(KeymapAction::ENABLE_MOUSE_CAMERA) != actions.end()) {
        mouseCameraEnabled = true;
        mInputManager->hideMouseCursor();
        mInputManager->disableGuiMouse();
    }

    if (mouseCameraEnabled) {
        mCamera->processMouse(mInputManager->getMousePosition());
        mCamera->processActions(frameTimeDelta, actions);
    }

    mCamera->drawDebugPanel();
}

glm::vec3
CameraManager::getPosition() {
    return mCamera->getPosition();
}

glm::mat4
CameraManager::getViewMatrix() {
    return mCamera->getViewMatrix();
}

glm::mat4
CameraManager::getProjectionMatrix() {
    return mCamera->getProjectionMatrix();
}
