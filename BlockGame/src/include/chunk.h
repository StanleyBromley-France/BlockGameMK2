#ifndef CHUNK_H
#define CHUNK_H

#include <map>

#include <unordered_map>
#include <vector>
#include <FastNoiseLite.h>
#include <glm/glm.hpp>

#include "biomes.h"
#include "../globals/hashUtils.h"

class Chunk {
public:
	Chunk(glm::vec3 position, Biomes::Biome currentBiome, FastNoiseLite noise, std::pair<int, int> gridPos);
	void SetUpBuffer(std::unordered_map<std::pair<int, int>, Chunk*, PairHash>& chunkMap);
	void RenderChunk();
	void Deallocate();
	glm::vec3 chunkPos;
	std::pair<int, int> chunkGridCoord;
	std::unordered_map<std::tuple<int, int, int>, bool, TupleHash> blockMap;
private:
	bool CheckOtherChunkForBlock(std::unordered_map<std::pair<int, int>, Chunk*, PairHash>& chunkMap, int& x, int& y, int& z);
	Biomes::Biome currentBiome;
	int modelMatricesSize = -1;
	unsigned int chunkBuffer = -1;
	int chunkSize = 16;
	int chunkHeight = 32;
};

#endif