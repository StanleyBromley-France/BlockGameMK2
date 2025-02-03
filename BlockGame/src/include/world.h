#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>

#include <unordered_map>
#include <utility>
#include <FastNoiseLite.h>
#include "chunk.h"
#include "../globals/hashUtils.h"
#include <vector>

class World {
private:
    const int mapWidth;
    const int mapHeight;
    FastNoiseLite terrainNoise;
    FastNoiseLite biomeNoise;

public:
    World(int width, int height, int seed);

    //std::vector<Chunk> chunks;

    std::unordered_map<std::pair<int, int>,  Chunk*, PairHash> chunkMap;
};

#endif 
