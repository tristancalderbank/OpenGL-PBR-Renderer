#pragma once

#include <vector>

#include "model.h"

/**
 * Scene is a global object that represents all the things in our current scene.
 */
struct Scene {
    std::vector<Model> mModels;
    std::vector<glm::vec3> mLightPositions;
    std::vector<glm::vec3> mLightColors;
};