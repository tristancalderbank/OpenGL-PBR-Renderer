#pragma once

#include <functional>

#include "engineconfig.h"
#include "inputmanager.h"

struct WindowSize {
    int width;
    int height;
};

class InputManager;

/**
 * Manages the window.
 */
class WindowManager {
public:
    WindowManager(EngineConfig &engineConfig);
    void startup(std::shared_ptr<InputManager> inputManager);
    void update();
    void clearResizeFlag();
    void shutdown();

    GLFWwindow* getWindow();
    bool windowShouldClose();
    bool windowResized();
    const WindowSize &getWindowSize();

private:
    EngineConfig mEngineConfig;
    std::shared_ptr<InputManager> mInputManager;

    GLFWwindow* mWindow;
    WindowSize mWindowSize;
    bool mWindowResized = false;
};