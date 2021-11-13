#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "material.h"
#include "vertex.h"

const int TEXTURE_UNIT_ALBEDO = 0;
const int TEXTURE_UNIT_METALLIC_ROUGHNESS = 1;
const int TEXTURE_UNIT_NORMAL = 2;
const int TEXTURE_UNIT_AMBIENT_OCCLUSION = 3;
const int TEXTURE_UNIT_EMISSIVE = 4;

/**
 * A mesh is a collection of geometry paired with a material.
 */
class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material material);
    void Draw(Shader& shader);

public: 
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	Material mMaterial;

private:
	void init();

private:
    // OpenGL data structures
    unsigned int mVAO, mVBO, mEBO;
};