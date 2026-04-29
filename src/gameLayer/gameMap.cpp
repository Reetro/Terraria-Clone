#include "gameMap.h"
#include <asserts.h>

void GameMap::create(int w, int h)
{
    *this = {};
    mapData.resize(w * h);
    tileData.resize(w * h);

    this->w = w;
    this->h = h;

    // Clear all block data
    for (Block &block : mapData)
    {
        block = {};
    }

    // Clear all tile data
    for (Tile &tile : tileData)
    {
        tile = {};
    }
}

Block &GameMap::getBlocUnsafe(int x, int y)
{
    permaAssertCommentDevelopement(mapData.size() == w * h, "Map data not initialized");

    permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getBlocUnsafe out of bounds error");

    return mapData[x + y * w];
}

Block *GameMap::getBlockSafe(int x, int y)
{
    permaAssertCommentDevelopement(mapData.size() == w * h, "Map data not initialized");

    if (x < 0 || y < 0 || x >= w || y >= h)
    {
        return nullptr;
    }

    return &mapData[x + y * w];
}

Tile &GameMap::getTileUnsafe(int x, int y)
{
    permaAssertCommentDevelopement(tileData.size() == w * h, "Wall data not initialized");
    permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getWallUnsafe out of bounds");

    return tileData[x + y * w];
}

Tile *GameMap::getTileSafe(int x, int y)
{
    if (x < 0 || y < 0 || x >= w || y >= h)
    {
        return nullptr;
    }

    return &tileData[x + y * w];
}