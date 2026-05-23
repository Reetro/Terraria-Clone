#pragma once
#include <blocks.h>
#include <random>
#include <vector>

enum class BiomeType
{
    Desert,
    Snow,
};

struct BiomeRegion
{
    int start;
    int end;
    BiomeType type;

    bool contains(int x) const { return x >= start && x <= end; }

    int blockDirt() const;
    int blockGrass() const;
    int blockStone() const;

    // Returns the Y depth below which biome stone should apply, or -1 to skip
    int stoneDepth(int x, int stoneHeight) const;
};

std::vector<BiomeRegion> generateBiomes(int worldWidth, std::ranlux24_base& rng);