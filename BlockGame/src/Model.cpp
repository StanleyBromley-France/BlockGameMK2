#include "Model.h"
#include <iostream>

ImportedModel::ImportedModel(const std::string& path, bool flipOnLoad, bool gamma)
    : gammaCorrection(gamma)
{
    stbi_set_flip_vertically_on_load(flipOnLoad); // sets flipping based on parameter
    loadModel(path);
    stbi_set_flip_vertically_on_load(false); // resets flipping state


}

void ImportedModel::Draw(Shader& shader)
{
    for (auto& mesh : meshes)
    {
        mesh.Draw(shader);
    }
}

// loads the model from the given file path, processes its meshes and textures
void ImportedModel::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    // checks for errors during model loading
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // extracts the directory path from the file path for texture loading
    directory = path.substr(0, path.find_last_of('/'));

    // processes the root node, which contains meshes and child nodes
    processNode(scene->mRootNode, scene);
}

// processes each node in the scene and processes meshes attached to it
void ImportedModel::processNode(aiNode* node, const aiScene* scene)
{
    // processes all meshes of the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene)); // Process each mesh and store it
    }

    //  process each child node of the current node
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

// processes a mesh by extracting vertex data and handling textures
Mesh ImportedModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // extracts vertex data for positions, normals, texture coordinates, tangents, and bitangents
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        // texture Coordinates
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            // Tangents and Bitangents
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex); // adds vertex to the vertex vector
    }

    // extracts indices for faces of the mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]); // adds face indices to the indices vector
        }
    }

    // Check if this texture is embedded in the .glb file
    const aiTexture* embeddedTexture = scene->GetEmbeddedTexture("*0");
    if (embeddedTexture)
    {
        // Handle embedded texture loading
        Texture texture;
        texture.id = LoadEmbeddedTexture(embeddedTexture);  // Use new method for embedded textures
        texture.type = aiTextureType_DIFFUSE;
        texture.path = "*0";
        textures.push_back(texture);
        textures_loaded.push_back(texture);
    }

    const aiTexture* embeddedTexture2 = scene->GetEmbeddedTexture("*1");
    if (embeddedTexture2)
    {
        // Handle embedded texture loading
        Texture texture;
        texture.id = LoadEmbeddedTexture(embeddedTexture2);  // Use new method for embedded textures
        texture.type = aiTextureType_DIFFUSE;
        texture.path = "*1";
        textures.push_back(texture);
        textures_loaded.push_back(texture);
    }

    const aiTexture* embeddedTexture3 = scene->GetEmbeddedTexture("*2");
    if (embeddedTexture3)
    {
        // Handle embedded texture loading
        Texture texture;
        texture.id = LoadEmbeddedTexture(embeddedTexture3);  // Use new method for embedded textures
        texture.type = aiTextureType_SPECULAR;
        texture.path = "*1";
        textures.push_back(texture);
        textures_loaded.push_back(texture);
    }

    // processes materials and load textures for the mesh
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // loads other types of textures (specular, normal, height)
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", scene);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", scene);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // returns the created mesh object
    return Mesh(vertices, indices, textures);
}


// loads material textures from the material object and checks for duplicates
std::vector<Texture> ImportedModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const aiScene* scene)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // skips loading the texture if it's already loaded
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // skips if texture already loaded
                break;
            }
        }

        // if texture is not loaded, load it from file
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // Store loaded texture
        }
    }
    return textures;
}

// loads a texture from file and returns its OpenGL texture ID
unsigned int ImportedModel::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        std::cout << "Loading texture: " << path << std::endl;

        GLenum format = (nrComponents == 1) ? GL_RED : (nrComponents == 3) ? GL_RGB : GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // sets texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // use nearest neighbour filtering for both minification and magnification
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE::" << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int ImportedModel::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
{
    // Create a texture from the embedded texture's data
    unsigned int textureID;
    glGenTextures(1, &textureID);

    // Get the texture data (Assimp stores it as a byte buffer)
    const aiTexture* texture = embeddedTexture;
    const void* textureData = texture->pcData;
    unsigned int textureSize = texture->mWidth; // Size is stored in mWidth for embedded textures

    int width, height, nrComponents;

    // If it's an embedded texture, we assume it's PNG or JPEG, so use stb_image to load it
    unsigned char* data = stbi_load_from_memory(static_cast<const unsigned char*>(textureData), textureSize, &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format = (nrComponents == 1) ? GL_RED : (nrComponents == 3) ? GL_RGB : GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // sets texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_EMBEDDED_TEXTURE" << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}