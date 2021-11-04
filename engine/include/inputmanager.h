#pragma once

#include <map>
#include <set>

#include "glfw3.h"

#include "engineconfig.h"
#include "windowmanager.h"

enum class KeymapAction {
    EXIT,
    ENABLE_MOUSE_CAMERA,
    DISABLE_MOUSE_CAMERA,

    // movement
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN
};

struct MousePosition {
    double x;
    double y;
};

class WindowManager;
class InputManager {
public:
    InputManager(EngineConfig &engineConfig, std::shared_ptr<WindowManager> windowManager);
    void startup();
    void update();
    void enableGuiMouse();
    void disableGuiMouse();
    void showMouseCursor();
    void hideMouseCursor();
    const std::set<KeymapAction> &getCurrentActions();
    const MousePosition &getMousePosition();
private:
    EngineConfig mEngineConfig;
    std::shared_ptr<WindowManager> mWindowManager;

    std::map<int, KeymapAction> mKeymap = {
            {GLFW_KEY_ESCAPE, KeymapAction::EXIT},
            {GLFW_KEY_1, KeymapAction::ENABLE_MOUSE_CAMERA},
            {GLFW_KEY_2, KeymapAction::DISABLE_MOUSE_CAMERA},
            {GLFW_KEY_W, KeymapAction::MOVE_FORWARD},
            {GLFW_KEY_A, KeymapAction::MOVE_LEFT},
            {GLFW_KEY_S, KeymapAction::MOVE_BACKWARD},
            {GLFW_KEY_D, KeymapAction::MOVE_RIGHT},
            {GLFW_KEY_SPACE, KeymapAction::MOVE_UP},
            {GLFW_KEY_LEFT_CONTROL, KeymapAction::MOVE_DOWN}
    };

    MousePosition mMousePosition;
    std::set<KeymapAction> mCurrentActions;
};