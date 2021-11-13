#pragma once

#include <memory>

#include "cubemapframebuffer.h"
#include "shader.h"
#include "hdricube.h"

/**
 * A cubemap texture created from an equirectangular image.
 */
class EquirectangularCubemap {
public:
    /**
     * Initialize an EquirectangularCubemap.
     * @param hdriPath path to an .hdr image in equirectangular projection format
     */
    EquirectangularCubemap(const std::string &engineRoot, const std::string &hdriPath);

    /**
     * Render the equirectangular cubemap.
     */
    void compute();

    /**
     * Get the GL texture ID of the computed cubemap.
     * @return
     */
    unsigned int getCubemapId();

private:
    const unsigned int cubemapWidth = 512;
    const unsigned int cubemapHeight = 512;

    std::unique_ptr<Shader> hdriShader;
    std::unique_ptr<HDRICube> hdriCube;
    std::unique_ptr<CubemapFramebuffer> framebuffer;
};