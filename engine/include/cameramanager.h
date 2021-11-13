#pragma once

#include <memory>

#include "camera.h"
#include "engineconfig.h"
#include "inputmanager.h"
#include "windowmanager.h"

/**
 * Manages creating and updating cameras.
 */
class CameraManager {
public:
    CameraManager(EngineConfig &engineConfig, std::shared_ptr<WindowManager> windowManager, std::shared_ptr<InputManager> inputManager);
    void startup();
    void update(float frameTimeDelta);
    void drawDebugPanel();

    glm::vec3 getPosition();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
private:
    EngineConfig mEngineConfig;
    std::shared_ptr<WindowManager> mWindowManager;
    std::shared_ptr<InputManager> mInputManager;

    std::unique_ptr<Camera> mCamera;
    bool mMouseCameraEnabled = true;
};