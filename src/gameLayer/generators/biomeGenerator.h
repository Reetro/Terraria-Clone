#pragma once
#include <blocks.h>
#include <random>
#include <vector>

enum class BiomeType
{
    Grassland,
    Desert
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
};

std::vector<BiomeRegion> generateBiomes(int worldWidth, std::ranlux24_base& rng);