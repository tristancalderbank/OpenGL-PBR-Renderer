#include "hdricube.h"

HDRICube::HDRICube(const std::string &hdriPath) : hdrTexture(HDRTexture(hdriPath))
{
    loadVertexData();
}

void
HDRICube::Draw(Shader &shader) {
    shader.setInt("hdri", 0);

    // draw mesh
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, hdrTexture.getId());
    glDrawArrays(GL_TRIANGLES, 0, HDRICUBE_NUM_TRIANGLES);
    glBindVertexArray(0);
};

void
HDRICube::loadVertexData() {
    // create our data structures
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW); // copy over the vertex data

    // setup the locations of vertex data
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}
