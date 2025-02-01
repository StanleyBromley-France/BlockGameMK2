#pragma once
#include "chunk.h"
#include "../globals/tupleHash.h"

#include <unordered_map>

class Map {
private:
    const int mapWidth;
    const int mapHeight;
    FastNoiseLite terrainNoise;
    FastNoiseLite biomeNoise;

public:
    Map(int width, int height, int seed);

    //std::vector<Chunk> chunks;

    std::unordered_map<std::tuple<int, int, int>, Chunk*, TupleHash> chunkMap;
};
