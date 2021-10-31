#include "skybox.h"

#include <glad/glad.h>

Skybox::Skybox(std::string textureDirectoryPath) {
    loadCubemapTextures(textureDirectoryPath);
    cube = std::make_unique<Cube>();
}

Skybox::Skybox(unsigned int textureId) : textureId(textureId) {
    cube = std::make_unique<Cube>();
}

void
Skybox::Draw() {

    // default depth buffer value is 1.0
    // skybox depth is 1.0 everywhere
    // need equality to make sure skybox passes depth test in default value places
    glDepthFunc(GL_LEQUAL);

    // draw mesh
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    cube->Draw();
    glDepthFunc(GL_LESS); // go back to default depth comparison
};

void
Skybox::loadCubemapTextures(std::string textureDirectoryPath) {

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