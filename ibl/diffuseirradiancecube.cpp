#include "diffuseirradiancecube.h"

DiffuseIrradianceCube::DiffuseIrradianceCube(unsigned int environmentCubemap) : environmentCubemap(environmentCubemap) {
    cube = std::make_unique<Cube>();
}

void
DiffuseIrradianceCube::Draw(Shader &shader) {
    shader.setInt("environmentCubemap", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, environmentCubemap);
    cube->Draw(shader);
};