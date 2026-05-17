#pragma once
#include <randomStuff.h>
#include <gameMap.h>

struct Worm
{
    float x, y;
    float angle;
    float length;
    double radius;
};

void spawnCaveWorm(int minCount, int maxCount, GameMap& map, std::ranlux24_base& rng, long seed, const std::vector<int>& surfaceHeights);