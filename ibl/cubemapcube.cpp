#include "cubemapcube.h"

CubemapCube::CubemapCube(unsigned int cubemap) : cubemap(cubemap) {
    cube = std::make_unique<Cube>();
}

void
CubemapCube::Draw(Shader &shader) {
    shader.setInt("cubemap", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    cube->Draw(shader);
};