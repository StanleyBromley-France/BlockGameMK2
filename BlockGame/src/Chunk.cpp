#include "include/chunk.h"
#include "helpers/include/glfwHelper.h"
#include "globals/tupleHash.h"

#include <algorithm>
#include <cstdlib>
#include <unordered_map>

Chunk::Chunk(glm::vec3 position, Biomes::Biome currentBiome, FastNoiseLite noise) : chunkPos(position), currentBiome(currentBiome){
    // Custom hash function for std::tuple<int, int, int>


    const float spacing = 1.0f;
    std::vector<glm::mat4> modelMatrices;
    std::unordered_map<std::tuple<int, int, int>, glm::mat4, TupleHash> blockMap;
    std::vector<int> heights;

    for (int x = 0; x < chunkSize; ++x) {
        for (int z = 0; z < chunkSize; ++z) {
            float noiseValue = noise.GetNoise((x + chunkPos.x) * currentBiome.noiseScale, (z + chunkPos.z) * currentBiome.noiseScale);
            int height = static_cast<int>((noiseValue + 1.0f) * currentBiome.heightScale);

            height = std::clamp(height, 1, chunkHeight);

            heights.push_back(height);

            for (int y = 0; y < height; ++y) {
                glm::vec3 position(x * spacing, y * spacing, z * spacing);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, position + chunkPos);

                // Store in hash map
                blockMap[{x, y, z}] = model;
            }
        }
    }

    // Directions to check: left, right, front, back, above, below
    std::vector<std::tuple<int, int, int>> directions = {
        {-1, 0, 0}, {1, 0, 0}, {0, 0, -1}, {0, 0, 1}, {0, -1, 0}, {0, 1, 0}
    };

    // Iterate through stored blocks and check for empty spaces
    for (const auto& [pos, model] : blockMap) {
        auto [x, y, z] = pos;
        bool hasEmptyNeighbor = false;

        for (const auto& [dx, dy, dz] : directions) {
            if (blockMap.find({ x + dx, y + dy, z + dz }) == blockMap.end()) {
                hasEmptyNeighbor = true;
                break;
            }
        }

        if (hasEmptyNeighbor) {
            modelMatrices.push_back(model);
        }
    }


    modelMatricesSize = modelMatrices.size();

    // configure instanced array
    // -------------------------
    glGenBuffers(1, &chunkBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, chunkBuffer);

    // pass the modelMatrices vector to OpenGL
    glBufferData(GL_ARRAY_BUFFER, modelMatricesSize * sizeof(glm::mat4), modelMatrices.data(), GL_STATIC_DRAW);

    glBindVertexArray(CubeMesh::GetInstance().GetMeshBuffers().getVAO());

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void Chunk::RenderChunk()
{
    CubeMesh::GetInstance().SwitchTexture(currentBiome.texture);

    glBindVertexArray(CubeMesh::GetInstance().GetMeshBuffers().getVAO());

    glBindBuffer(GL_ARRAY_BUFFER, chunkBuffer);

    Shader& shader = CubeMesh::GetInstance().GetShader(currentBiome.texture);

    // activates shader
    shader.use();

    // pass projection matrix to shader
    shader.SetMat4("projectionMatrix", GLFWHelper::Projection());

    // camera/view transformation
    glm::mat4 view = glm::mat4(1.0f);
    view = GLFWHelper::LookAt();
    shader.SetMat4("viewMatrix", view);


    // Render the chunk using instanced drawing
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(36), GL_UNSIGNED_INT, 0, modelMatricesSize);
    glBindVertexArray(0);
}

void Chunk::Deallocate()
{
    CubeMesh::GetInstance().GetMeshBuffers().DeAllocate();
}
