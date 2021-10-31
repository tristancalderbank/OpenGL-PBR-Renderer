#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "stb_image/stb_image.h"

#include "cube.h"
#include "hdrtexture.h"
#include "shader.h"

/**
 * A unit cube textured with an equirectangular HDR image.
 */
class HDRICube {

public:
    HDRICube(const std::string &hdriPath);
    void Draw(Shader &shader);

private:
    std::unique_ptr<Cube> cube;
    HDRTexture hdrTexture;
};