#pragma once

#include "rendermanager.h"

/**
 * Top level object that orchestrates/contains all subsystems.
 */
class Engine {
public:
    void startup();
    void run();
private:
    RenderManager renderManager;
};