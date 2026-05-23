#pragma once
#include <gameMap.h>
#include <randomStuff.h>
#include "biomeGenerator.h"
#include <vector>

struct TerrainNoiseArrays
{
    float* dirtNoise  = nullptr;
    float* stoneNoise = nullptr;
    int width         = 0;

    ~TerrainNoiseArrays();
};

struct TerrainConfig
{
    int dirtOffsetStart  = -5;
    int dirtOffsetEnd    = 35;
    int stoneHeightStart = 50;
    int stoneHeightEnd   = 170;
};

TerrainNoiseArrays generateTerrainNoise(int w, long& seed);

void generateTerrain(
    GameMap& gameMap,
    int w,
    int h,
    const TerrainNoiseArrays& noise,
    const std::vector<BiomeRegion>& biomes,
    std::ranlux24_base& rng,
    const TerrainConfig& config = {}
);

std::vector<int> buildSurfaceHeights(
    int w,
    const TerrainNoiseArrays& noise,
    const TerrainConfig& config = {}
);