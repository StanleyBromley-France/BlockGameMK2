#pragma once
#include "Chunk.h"
#include <FastNoiseLite.h>
#include <vector>

class Map {
private:
    const int mapWidth;
    const int mapHeight;
    FastNoiseLite terrainNoise;
    FastNoiseLite biomeNoise;

public:
    Map(int width, int height, int seed);

    std::vector<Chunk> chunks;
};
