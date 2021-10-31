#include "engine.h"

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

    stbi_set_flip_vertically_on_load(false);
    scene->mModels.push_back(Model("resources/helmet/DamagedHelmet.gltf"));
    stbi_set_flip_vertically_on_load(true);

    engine.run();

    return 0;
}