#pragma once

#include <memory>

#include "mipmapcubemapframebuffer.h"
#include "shader.h"
#include "hdricube.h"

class SpecularMap {

public:
    /**
     * Initialize a specular map.
     */
    SpecularMap(const std::string &engineRoot, const unsigned int environmentCubemapId);

    /**
     * Render the pre-filtered environment map.
     */
    void computePrefilteredEnvMap();

    /**
     * Get the GL texture ID of the computed pre-filtered environment cubemap.
     * @return
     */
    unsigned int getPrefilteredEnvMapId();

private:
    const unsigned int prefilteredEnvMapMipLevels = 5;
    const unsigned int prefilteredEnvMapWidth = 128;
    const unsigned int prefilteredEnvMapHeight = 128;

    const unsigned int environmentCubemapId;

    std::unique_ptr<Shader> prefilteredEnvMapShader;
    std::unique_ptr<MipmapCubemapFramebuffer> prefilteredEnvMapFramebuffer;
};