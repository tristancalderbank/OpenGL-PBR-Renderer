#pragma once

#include "rendermanager.h"

/**
 * Top level object that orchestrates/contains all subsystems.
 */
class Engine {
public:
    void startup(std::shared_ptr<Scene> scene);
    void run();
private:
    RenderManager renderManager;
};