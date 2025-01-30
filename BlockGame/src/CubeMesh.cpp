#include "CubeMesh.h"

Shader* CubeMesh::ShaderMixed = nullptr;
Shader* CubeMesh::ShaderSingle = nullptr;

unsigned int CubeMesh::sandTexture = NULL;
unsigned int CubeMesh::dirtTexture = NULL;
unsigned int CubeMesh::stoneTexture = NULL;


// sets up cube model class, called first time getInstance is run
CubeMesh::CubeMesh()
{
    if (ShaderSingle == nullptr)
        ShaderSingle = new Shader("shaders/vertex_shader.glsl", "shaders/frag_single_texture.glsl");

    if (ShaderMixed == nullptr)
        ShaderMixed = new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    static float vertices[] = {
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

    static unsigned int indices[] = {
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

    if (sandTexture == NULL) {
        sandTexture = GladHelper::loadTexture("images/sand.jpg");
        dirtTexture = GladHelper::loadTexture("images/dirt.jpg");
        stoneTexture = GladHelper::loadTexture("images/stone.jpg");
    }

    //unsigned int texture2 = GladHelper::loadTexture("awesomeface.png", false);

    ShaderSingle->use();
    ShaderSingle->SetInt("texture1", 0); // assigns texture1 to texture unit 0


    ShaderMixed->use();
    ShaderMixed->SetInt("texture1", 0); // assigns texture1 to texture unit 0
    ShaderMixed->SetInt("texture2", 1); // assigns texture1 to texture unit 0

    ShaderMixed->use();
    glBindTexture(GL_TEXTURE_2D, dirtTexture); // GL_TEXTURE0
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sandTexture); // GL_TEXTURE1

}

Shader* CubeMesh::GetShader(Texture texture)
{
    if(texture != Texture::STONE)
        return ShaderSingle;

    return ShaderMixed;
}

GladHelper::MeshBuffers& CubeMesh::GetMeshBuffers()
{
    return MeshBuffers;
}

void CubeMesh::SwitchTexture(Texture texture)
{


    switch (texture)
    {
    case Texture::SAND:
        ShaderSingle->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sandTexture);
        break;
    case Texture::DIRT:
        ShaderMixed->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dirtTexture);
        break;
    case Texture::STONE:
        ShaderMixed->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, stoneTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, dirtTexture);
        break;
    }
}
