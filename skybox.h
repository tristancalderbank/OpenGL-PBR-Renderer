#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <glad/glad.h>
#include "lib/stb_image/stb_image.h"
#include "shader.h"

const unsigned int SKYBOX_CUBE_NUM_TRIANGLES = 36;

class Skybox {

public:
	Skybox(std::string textureDirectoryPath) {
		loadCubemapTextures(textureDirectoryPath);
        loadVertexData();
	};

    void Draw(Shader &shader) {
        shader.setInt("skybox", 0); // set skybox sampler to slot 0

        // default depth buffer value is 1.0
        // skybox depth is 1.0 everywhere
        // need equality to make sure skybox passes depth test in default value places 
        glDepthFunc(GL_LEQUAL); 

		// draw mesh
		glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
		glDrawArrays(GL_TRIANGLES, 0, SKYBOX_CUBE_NUM_TRIANGLES);
		glBindVertexArray(0);

        glDepthFunc(GL_LESS); // go back to default depth comparison
    };

private: 
    unsigned int textureId;
    unsigned int VAO, VBO;

    std::vector<float> vertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

	std::vector<std::string> faceTextureFileNames = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	
	void loadCubemapTextures(std::string textureDirectoryPath) {

        stbi_set_flip_vertically_on_load(false); // cubemaps in OpenGL are weird and don't require flip

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

		int width, height, numChannels;

		for (unsigned int i = 0; i < faceTextureFileNames.size(); i++) {
			std::string path = textureDirectoryPath + '/' + faceTextureFileNames[i];
			unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

			if (!data) {
				std::cout << "Cubemap texture data failed to load for path: " << path << std::endl;
				return;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        stbi_set_flip_vertically_on_load(true);
	}

    void loadVertexData() {
		// create our data structures
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO); // use this VAO for subsequent calls

		glBindBuffer(GL_ARRAY_BUFFER, VBO); // use this VBO for subsequent calls
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW); // copy over the vertex data

		// setup the locations of vertex data
		// positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);
    }
	
};