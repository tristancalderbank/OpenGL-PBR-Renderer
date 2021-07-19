#pragma once

#include <vector>

#include <glad/glad.h>
#include "shader.h"

const unsigned int QUAD_NUM_TRIANGLES = 6;

class FullscreenQuad {

public:
	FullscreenQuad() {
        loadVertexData();
	};

    void Draw(Shader &shader, unsigned int colorTextureId) {
		shader.use();
	
		glActiveTexture(GL_TEXTURE0);
		shader.setInt("colorTexture", 0);
		glBindTexture(GL_TEXTURE_2D, colorTextureId);

		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM_TRIANGLES);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
    };

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

    void loadVertexData() {
		// create our data structures
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao); // use this VAO for subsequent calls

		glBindBuffer(GL_ARRAY_BUFFER, vbo); // use this VBO for subsequent calls
		glBufferData(GL_ARRAY_BUFFER, vertexAttributes.size() * sizeof(float), &vertexAttributes[0], GL_STATIC_DRAW); // copy over the vertex data

		// setup the locations of vertex data
		// positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		// texture coordinates
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindVertexArray(0);
    }
	
};