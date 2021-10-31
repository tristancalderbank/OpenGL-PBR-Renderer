#include "ibl/hdricube.h"

#include <glad/glad.h>

HDRICube::HDRICube(const std::string &hdriPath) : hdrTexture(HDRTexture(hdriPath))
{
    cube = std::make_unique<Cube>();
}

void
HDRICube::Draw(Shader &shader) {
    shader.setInt("hdri", 0);

    // draw mesh
    glBindTexture(GL_TEXTURE_2D, hdrTexture.getId());
    cube->Draw();
};