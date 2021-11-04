#pragma once

#include <memory>

#include "constants.h"
#include "texture.h"

struct Material {
    bool useTextureAlbedo = false;
    bool useTextureMetallicRoughness = false;
    bool useTextureNormal = false;
    bool useTextureAmbientOcclusion = false;
    bool useTextureEmissive = false;

    glm::vec3 albedo = constants::colorRed;
    float metallic = 1.0f;
    float roughness = 0.0f;
    float ambientOcclusion = 1.0f;
    glm::vec3 emissive = glm::vec3(0.0, 0.0, 0.0);

    std::shared_ptr<Texture> textureAlbedo;
    std::shared_ptr<Texture> textureMetallicRoughness;
    std::shared_ptr<Texture> textureNormal;
    std::shared_ptr<Texture> textureAmbientOcclusion;
    std::shared_ptr<Texture> textureEmissive;
};