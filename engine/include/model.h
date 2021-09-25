#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include "stb_image/stb_image.h"

#include <map>
#include "mesh.h"

class Model {
public:
	Model(std::string path);
	void Draw(Shader& shader);

private:
	void loadModel(std::string path);

	// recursively load all meshes in the node tree 
	void processNode(aiNode* node, const aiScene* scene);

	// convert assimp mesh to our own mesh class
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	// loads the first texture of given type
	Texture loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* fileName, std::string directory, aiTextureType type);

private:
    // data
    std::vector<Mesh> meshes;
    std::string directory;
    std::map<std::string, Texture> texturesLoaded;
};