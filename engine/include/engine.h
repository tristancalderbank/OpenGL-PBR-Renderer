#pragma once

#include "rendermanager.h"
#include "cameramanager.h"
#include "engineconfig.h"
#include "inputmanager.h"
#include "windowmanager.h"

/**
 * Top level object that orchestrates/contains all subsystems.
 */
class Engine {
public:
    Engine(EngineConfig &config);
    void startup(std::shared_ptr<Scene> scene);
    void run();
    void shutdown();
private:
    EngineConfig mEngineConfig;
    std::shared_ptr<WindowManager> mWindowManager;
    std::shared_ptr<InputManager> mInputManager;
    std::shared_ptr<CameraManager> mCameraManager;
    std::shared_ptr<RenderManager> mRenderManager;
};