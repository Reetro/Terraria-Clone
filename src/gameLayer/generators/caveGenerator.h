#pragma once
#include <gameMap.h>

struct CaveConfig
{
    float threshold = 0.35f;
};

void carveCaves(GameMap& gameMap, int w, int h, long& seed, const CaveConfig& config = {});