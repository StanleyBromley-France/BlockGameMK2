#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <STB/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class ImportedModel
{
public:
    ImportedModel(const std::string& path, bool flipOnLoad = false, bool gamma = false);

    void Draw(Shader& shader);

private:
    void loadModel(const std::string& path);

    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const aiScene* scene);

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

    unsigned int LoadEmbeddedTexture(const aiTexture* embeddedTexture);

    std::vector<Texture> textures_loaded;

    std::vector<Mesh> meshes;

    std::string directory;

    bool gammaCorrection;
};

#endif
