#pragma once
#include <random>

#include "gameMap.h"

struct OreSettings
{
    int blockType;
    float spawnChance;
    int minDepth;
};

void spawnOres(GameMap &gameMap, int w, int h, const std::vector<OreSettings> &ores,std::ranlux24_base &rng);
