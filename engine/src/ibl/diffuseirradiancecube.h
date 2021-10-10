#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <glad/glad.h>

#include "cube.h"
#include "shader.h"

class DiffuseIrradianceCube {

public:
    DiffuseIrradianceCube(unsigned int environmentCubemap);
    void Draw(Shader &shader);

private:
    std::unique_ptr<Cube> cube;
    unsigned int environmentCubemap;
};