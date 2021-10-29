#pragma once

#include <vector>

#include <glad/glad.h>
#include "shader.h"

const unsigned int QUAD_NUM_TRIANGLES = 6;

class FullscreenQuad {
public:
	FullscreenQuad();
    void Draw();

private:
    void loadVertexData();

private: 
    unsigned int vao;
    unsigned int vbo;

    std::vector<float> vertexAttributes = {
		// positions   // textureCoordinates
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
    };
};