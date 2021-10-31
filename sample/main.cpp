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

//    auto sphereMaterial = std::make_shared<Material>();
//
//    sphereMaterial->useTextureAlbedo = false;
//    sphereMaterial->useTextureMetallicRoughness = false;
//    sphereMaterial->useTextureNormal = false;
//    sphereMaterial->useTextureAmbientOcclusion = false;
//    sphereMaterial->useTextureEmissive = false;
//    sphereMaterial->albedo = glm::vec3(0.0, 0.0, 0.0);
//    sphereMaterial->metallic = 1.0f;
//    sphereMaterial->roughness = 0.1f;
//
//    auto sphereModel = std::make_shared<Model>("resources/sphere/sphere.gltf", true);
//    auto sphere = Entity(sphereModel);

    auto helmetModel = std::make_shared<Model>("resources/helmet/DamagedHelmet.gltf", false);
    auto helmet = Entity(helmetModel);
    helmet.setOrientation(glm::angleAxis(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f)));

    scene->mEntities.push_back(helmet);

    engine.run();

    return 0;
}