#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "stb_image/stb_image.h"

#include "cube.h"

class Skybox {

public:
    /**
     * Create a skybox by loading 6 cube face textures.
     * @param textureDirectoryPath
     */
	Skybox(std::string textureDirectoryPath);

	/**
	 * Create a skybox from an existing cubemap texture.
	 * @param textureId
	 */
    Skybox(unsigned int textureId);
    void Draw();

private:
    void loadCubemapTextures(std::string textureDirectoryPath);

private:
    std::unique_ptr<Cube> cube;
    unsigned int textureId;

	std::vector<std::string> faceTextureFileNames = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
};