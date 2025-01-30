#ifndef BIOMES_H
#define BIOMES_H

#include "CubeMesh.h"
#include <vector>
#include <algorithm>
#include <string>

namespace Biomes {

    struct Biome {
        std::string name;
        float noiseScale;
        int heightScale;
        CubeMesh::Texture texture;
        float minNoiseThreshold;
        float maxNoiseThreshold;
    };

    // defines the various biomes with overlapping thresholds for smoother transitions
    inline const Biome mountainousBiome = {
        "Mountain", 0.7f, 10, CubeMesh::Texture::STONE, 0.5f, 0.8f
    };

    inline const Biome hillyBiome = {
        "Hilly", 0.12f, 6, CubeMesh::Texture::DIRT, 0.2f, 0.5f
    };

    inline const Biome desertBiome = {
        "Desert", 0.1f, 5, CubeMesh::Texture::SAND, -1.0f, 0.2f
    };

    // list of available biomes, ordered by the minimum noise threshold for easier lookup
    inline const std::vector<Biome> biomes = {
        mountainousBiome,
        hillyBiome,
        desertBiome
    };

    // function to get the biome based on a noise value
    inline const Biome& getBiomeFromNoise(float noiseValue) {
        // iterate over the biomes to find the first matching one within its noise range
        for (const Biome& biome : biomes) {
            if (noiseValue >= biome.minNoiseThreshold && noiseValue < biome.maxNoiseThreshold) {
                return biome;
            }
        }
        // defaults to the last biome (lowest threshold) if no match is found
        return biomes.back();
    }
}

#endif