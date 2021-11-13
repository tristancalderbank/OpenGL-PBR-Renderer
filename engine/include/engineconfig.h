#pragma once

#include <string>

/**
 * Engine config accessible by all subsystems.
 */
struct EngineConfig {
    const char* glslVersion = "#version 330";

    int initialViewportWidth = 3000;
    int initialViewportHeight = 2000;
    float imguiFontScale = 1.0;

    std::string engineRootPath;
    std::string hdriPath;
};