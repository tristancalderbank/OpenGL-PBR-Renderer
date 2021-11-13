#pragma once

#include "glm/gtc/quaternion.hpp"

#include "model.h"

/**
 * A model paired with a position, scale, and orientation.
 */
class Entity {
public:
    Entity(std::shared_ptr<Model> model);

    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();

    void setScale(glm::vec3 scale);
    glm::vec3 getScale();

    void setOrientation(glm::quat orientation);
    glm::quat getOrientation();

    std::shared_ptr<Model> getModel();

private:
    glm::vec3 mPosition;
    glm::vec3 mScale = glm::vec3(1.0, 1.0, 1.0);
    glm::quat mOrientation;
    std::shared_ptr<Model> mModel;
};