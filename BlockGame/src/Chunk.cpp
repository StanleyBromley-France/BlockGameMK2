#include "Chunk.h"

#include <algorithm>
#include <cstdlib>

Chunk::Chunk(glm::vec3 position, Biomes::Biome currentBiome, FastNoiseLite noise) : chunkPos(position), currentBiome(currentBiome){
    const float spacing = 1.0f;


    // Dynamically allocate the cubes array on the heap
    cubes = new CubeInstance * *[chunkSize];
    for (int x = 0; x < chunkSize; ++x) {
        cubes[x] = new CubeInstance * [chunkSize];
        for (int z = 0; z < chunkSize; ++z) {
            cubes[x][z] = new CubeInstance[chunkHeight];
        }
    }

    // Initialises the cubes with positions and random heights
    for (int x = 0; x < chunkSize; ++x) {
        for (int z = 0; z < chunkSize; ++z) {
            // Generates a noise value for this (x, z) coordinate
            float noiseValue = noise.GetNoise((x + chunkPos.x) * currentBiome.noiseScale, (z + chunkPos.z) * currentBiome.noiseScale);

            // Maps noise value from [-1, 1] to [0, 5] for height
            int height = static_cast<int>((noiseValue + 1.0f) * currentBiome.heightScale);
            // Clamps the height to the range [0, 5]
            height = std::clamp(height, 1, chunkHeight);

            // Populates the cubes up to the random height
            for (int y = 0; y < height; ++y) {
                glm::vec3 position(x * spacing, y * spacing, z * spacing);
                cubes[x][z][y] = CubeInstance(position + chunkPos);

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, position + chunkPos);
                modelMatrices.push_back(model);  // Add the model matrix to the vector
            }
        }
    }

    Mesh = CubeMesh();

    // configure instanced array
    // -------------------------
    glGenBuffers(1, &chunkBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, chunkBuffer);

    // pass the modelMatrices vector to OpenGL
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data(), GL_STATIC_DRAW);

    glBindVertexArray(Mesh.GetMeshBuffers().getVAO());

    // set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void Chunk::RenderChunk()
{
    Mesh.SwitchTexture(currentBiome.texture);

    glBindVertexArray(Mesh.GetMeshBuffers().getVAO());

    Shader* shader = Mesh.GetShader(currentBiome.texture);

    // activates shader
    shader->use();

    // pass projection matrix to shader
    shader->SetMat4("projection", GLFWHelper::Projection());

    // camera/view transformation
    glm::mat4 view = glm::mat4(1.0f);
    view = GLFWHelper::LookAt();
    shader->SetMat4("view", view);


    // Render the chunk using instanced drawing
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(36), GL_UNSIGNED_INT, 0, modelMatrices.size());
    glBindVertexArray(0);
}

void Chunk::Deallocate()
{
    Mesh.GetMeshBuffers().DeAllocate();
}
