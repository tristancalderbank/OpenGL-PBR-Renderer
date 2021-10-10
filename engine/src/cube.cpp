#include "cube.h"

Cube::Cube() {
    loadVertexData();
}

void
Cube::Draw() {
    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
};

void
Cube::loadVertexData() {
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
