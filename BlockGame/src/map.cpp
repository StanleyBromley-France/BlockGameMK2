#include "include/map.h"

const int CHUNK_SIZE_X = 16;
const int CHUNK_SIZE_Y = 16;
const int CHUNK_SIZE_Z = 256;

Map::Map(int width, int height, int seed)
    : mapWidth(width), mapHeight(height)
{
    // Configure terrain noise generator
    terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    terrainNoise.SetFrequency(0.1f);
    terrainNoise.SetSeed(seed);

    // Configure biome noise generator
    biomeNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    biomeNoise.SetFrequency(0.5f); // Larger areas for biomes
    biomeNoise.SetSeed(seed + 1);

    int chunkSize = 16;
    int chunkHeight = 256;
    int spacing = 16;

    for (int x = 0; x < mapWidth; x++) {
        for (int z = 0; z < mapHeight; z++) {
            glm::vec3 position(x * spacing, 0.0f, z * spacing);
            Biomes::Biome biome = Biomes::getBiomeFromNoise(biomeNoise.GetNoise(x * 0.5f, z * 0.5f));

            if (chunkMap.find({ x, 0, z }) == chunkMap.end()) {
                // Create new chunk and add it to the map
                chunkMap[{x, 0, z}] = new Chunk(position, biome, terrainNoise);
            }
        }
    }
}



