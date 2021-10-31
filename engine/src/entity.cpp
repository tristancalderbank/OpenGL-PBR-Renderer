#include "entity.h"

Entity::Entity(std::shared_ptr<Model> model) : mModel(model)
{

}

void Entity::setPosition(glm::vec3 position)
{
    mPosition = position;
}

glm::vec3 Entity::getPosition()
{
    return mPosition;
}

void Entity::setScale(glm::vec3 scale)
{
    mScale = scale;
}

glm::vec3 Entity::getScale()
{
    return mScale;
}

void Entity::setOrientation(glm::quat orientation)
{
    mOrientation = orientation;
}

glm::quat Entity::getOrientation()
{
    return mOrientation;
}

std::shared_ptr<Model> Entity::getModel()
{
    return mModel;
}
