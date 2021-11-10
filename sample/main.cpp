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
    EngineConfig engineConfig;
    engineConfig.imguiFontScale = 1.0f;
    engineConfig.initialViewportWidth = 1200;
    engineConfig.initialViewportHeight = 800;
    engineConfig.hdriPath = "resources/hdr/circus_backstage.hdr";

    Engine engine(engineConfig);
    auto scene = std::make_shared<Scene>();

    engine.startup(scene);

    scene->mLightPositions.push_back(glm::vec3(0.0f, 0.0f, 10.0f));
    scene->mLightColors.push_back(glm::vec3(150.0f, 150.0f, 150.0f));

//    auto helmetModel = std::make_shared<Model>("resources/helmet/DamagedHelmet.gltf", false);
//    auto helmet = Entity(helmetModel);
//    helmet.setOrientation(glm::angleAxis(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f)));
//    scene->mEntities.push_back(helmet);



    float spacing = 2.5;
    float values[] = {0.0, 0.3, 0.5, 0.7, 1.0};

    for (auto i = 0; i < 5; i++) {
        for (auto j = 0; j < 5; j++) {
            auto sphereMaterial = std::make_shared<Material>();

            sphereMaterial->useTextureAlbedo = false;
            sphereMaterial->useTextureMetallicRoughness = false;
            sphereMaterial->useTextureNormal = false;
            sphereMaterial->useTextureAmbientOcclusion = false;
            sphereMaterial->useTextureEmissive = false;
            sphereMaterial->albedo = glm::vec3(1.0, 1.0, 1.0);
            sphereMaterial->metallic = values[i];
            sphereMaterial->roughness = values[j];

            auto sphereModel = std::make_shared<Model>("resources/sphere/sphere.gltf", sphereMaterial, true);
            auto sphere = Entity(sphereModel);

            sphere.setPosition(glm::vec3(0.0 + j * spacing, 0.0 + i * spacing, 0.0));

            scene->mEntities.push_back(sphere);
        }
    }

    engine.run();
    engine.shutdown();

    return 0;
}