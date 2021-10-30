#include "mesh.h"

#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material material) {
    this->vertices = vertices;
    this->indices = indices;
    this->material = material;

    init();
}

void
Mesh::Draw(Shader& shader) {

    // albedo
    shader.setBool("material.useTextureAlbedo", material.useTextureAlbedo);
    shader.setVec3("material.albedo", material.albedo);
    if (material.useTextureAlbedo) {
        glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_ALBEDO);
        shader.setInt("material.textureAlbedo", TEXTURE_UNIT_ALBEDO);
        glBindTexture(GL_TEXTURE_2D, material.textureAlbedo->id);
    }

    shader.setBool("material.useTextureMetallicRoughness", material.useTextureMetallicRoughness);
    shader.setFloat("material.metallic", material.metallic);
    shader.setFloat("material.roughness", material.roughness);
    if (material.textureMetallicRoughness) {
        glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_METALLIC_ROUGHNESS);
        shader.setInt("material.textureMetallicRoughness", TEXTURE_UNIT_METALLIC_ROUGHNESS);
        glBindTexture(GL_TEXTURE_2D, material.textureMetallicRoughness->id);
    }

    shader.setBool("material.useTextureNormal", material.useTextureNormal);
    if (material.useTextureNormal) {
        glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_NORMAL);
        shader.setInt("material.textureNormal", TEXTURE_UNIT_NORMAL);
        glBindTexture(GL_TEXTURE_2D, material.textureNormal->id);
    }

    shader.setBool("material.useTextureAmbientOcclusion", material.useTextureAmbientOcclusion);
    shader.setFloat("material.ambientOcclusion", material.ambientOcclusion);
    if (material.useTextureAmbientOcclusion) {
        glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_AMBIENT_OCCLUSION);
        shader.setInt("material.textureAmbientOcclusion", TEXTURE_UNIT_AMBIENT_OCCLUSION);
        glBindTexture(GL_TEXTURE_2D, material.textureAmbientOcclusion->id);
    }

    shader.setBool("material.useTextureEmissive", material.useTextureEmissive);
    shader.setVec3("material.emissive", material.emissive);
    if (material.useTextureEmissive) {
        glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_EMISSIVE);
        shader.setInt("material.textureEmissive", TEXTURE_UNIT_EMISSIVE);
        glBindTexture(GL_TEXTURE_2D, material.textureEmissive->id);
    }

    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void
Mesh::init() {
    // create our data structures
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // use this VAO for subsequent calls

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // use this VBO for subsequent calls
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); // copy over the vertex data

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // use this EBO for subsequent calls
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); // copy over the index data

    // setup the locations of vertex data
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

    // tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}