#include "engine.h"

#include "glm/gtc/quaternion.hpp"

#include "scene.h"

/**
 *
 * @param argc number of args
 * @param argv name, viewport_width, viewport_height
 * @return
 */
int main(int argc, const char * argv[])
{
    Engine engine;
    auto scene = std::make_shared<Scene>();

    engine.startup(scene);

    scene->mLightPositions.push_back(glm::vec3(0.0f, 0.0f, 10.0f));
    scene->mLightColors.push_back(glm::vec3(150.0f, 150.0f, 150.0f));

    auto helmetModel = std::make_shared<Model>("resources/helmet/DamagedHelmet.gltf", false);
    auto helmet = Entity(helmetModel);
    helmet.setOrientation(glm::angleAxis(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f)));
    scene->mEntities.push_back(helmet);

    engine.run();

    return 0;
}