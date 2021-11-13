#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include "stb_image/stb_image.h"

#include <map>
#include "mesh.h"

/**
 * A collection of meshes.
 */
class Model {
public:
    /**
     * Load a glTF 2.0 model.
     * @param path
     */
	Model(std::string path);
    Model(std::string path, bool flipTexturesVertically);

    /**
     * Load a glTF 2.0 model using a provided material. This will ignore any material
     * present in the model file.
     * @param path
     */
    Model(std::string path, std::shared_ptr<Material> material, bool flipTexturesVertically);
	void Draw(Shader& shader);

private:
	void loadModel(std::string path, bool flipTexturesVertically);

	// recursively load all meshes in the node tree 
	void processNode(aiNode* node, const aiScene* scene);

	// convert assimp mesh to our own mesh class
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	// loads the first texture of given type
    std::shared_ptr<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type);
	unsigned int textureFromFile(const char* fileName, std::string directory, aiTextureType type);

private:
    // data
    std::vector<Mesh> mMeshes;
    std::string mDirectory;
    std::map<std::string, std::shared_ptr<Texture>> mTexturesLoaded;
    std::shared_ptr<Material> mMaterialOverride;
};