#pragma once

#include <memory>

#include "cubemapframebuffer.h"
#include "shader.h"
#include "hdricube.h"

class DiffuseIrradianceMap {

public:
    /**
     * Initialize a diffuse irradiance map.
     * @param hdriPath path to an .hdr image in equirectangular projection format
     */
    DiffuseIrradianceMap(const std::string &engineRoot, const std::string &hdriPath);

    /**
     * Render the diffuse irradiance map.
     */
    void compute();

    /**
     * Get the GL texture ID of the computed cubemap.
     * @return
     */
    unsigned int getCubemapId();

    /**
     * Get the environment cubemap computed from the equirectangular HDR image.
     */
    unsigned int getEnvCubemapId();

private:
    const unsigned int cubemapWidth = 512;
    const unsigned int cubemapHeight = 512;
    const unsigned int diffuseIrradianceMapWidth = 32;
    const unsigned int diffuseIrradianceMapHeight = 32;

    std::unique_ptr<Shader> hdriShader;
    std::unique_ptr<Shader> diffuseIrradianceShader;

    std::unique_ptr<CubemapFramebuffer> environmentFramebuffer;
    std::unique_ptr<CubemapFramebuffer> diffuseIrradianceFramebuffer;
    std::unique_ptr<HDRICube> hdriCube;
};