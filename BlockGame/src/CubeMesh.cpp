#include "include/cubeMesh.h"

// static method to get the singleton instance
CubeMesh& CubeMesh::GetInstance()
{
    static CubeMesh instance;
    return instance;
}

// sets up cube model class, called first time getInstance is run
// sets up cube model class, called first time getInstance is run
CubeMesh::CubeMesh() : Shader("shaders/vertex_shader.glsl", "shaders/frag_single_texture.glsl")
{
    float vertices[] = {
        // Positions          // Texture Coords

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // Bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // Top-left

        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // Bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Top-left

        // Left face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Top-left

        // Right face
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Top-left

         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
          0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // Bottom-right
          0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
         -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  // Top-left

         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left
          0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  // Bottom-right
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // Top-right
         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f   // Top-left
    };

    unsigned int indices[] = {
        // Front face
        0, 1, 2,  2, 3, 0,
        // Back face
        4, 7, 6, 6, 5, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 15, 14, 14, 13, 12,
        // Bottom face
        16, 17, 18, 18, 19, 16,
        // Top face
        20, 23, 22, 22, 21, 20
    };

    MeshBuffers = GladHelper::MeshBuffers();
    MeshBuffers.SetupMeshBuffers(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int));

    sandTexture = GladHelper::loadTexture("images/sand.png");
    dirtTexture = GladHelper::loadTexture("images/dirt.png");
    stoneTexture = GladHelper::loadTexture("images/stone.png");

    Shader.use();
    Shader.SetInt("texture1", 0); // assigns texture1 to texture unit 0

}

Shader& CubeMesh::GetShader(Texture texture)
{
    if(texture != Texture::STONE)
        return Shader;

    return Shader;
}

GladHelper::MeshBuffers& CubeMesh::GetMeshBuffers()
{
    return MeshBuffers;
}

void CubeMesh::SwitchTexture(Texture texture)
{
    Shader.use();

    switch (texture)
    {
    case Texture::SAND:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sandTexture);
        Shader.SetInt("u_isGrayscale", 0);

        break;
    case Texture::DIRT:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dirtTexture);
        Shader.SetInt("u_isGrayscale", 0);
        break;
    case Texture::STONE:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, stoneTexture);
        Shader.SetInt("u_isGrayscale", 1);
        break;
    }
}
