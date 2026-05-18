#include "structure.h"
#include <asserts.h>

void Structure::create(int w, int h)
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

Block &Structure::getBlocUnsafe(int x, int y)
{
    permaAssertCommentDevelopement(mapData.size() == w * h, "Structure data not initialized");

    permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getBlocUnsafe out of bounds error");

    return mapData[x + y * w];
}

Block *Structure::getBlockSafe(int x, int y)
{
    permaAssertCommentDevelopement(mapData.size() == w * h, "Structure data not initialized");

    if (x < 0 || y < 0 || x >= w || y >= h)
    {
        return nullptr;
    }

    return &mapData[x + y * w];
}

Tile &Structure::getTileUnsafe(int x, int y)
{
    permaAssertCommentDevelopement(tileData.size() == w * h, "Wall data not initialized");
    permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getWallUnsafe out of bounds");

    return tileData[x + y * w];
}

Tile *Structure::getTileSafe(int x, int y)
{
    if (x < 0 || y < 0 || x >= w || y >= h)
    {
        return nullptr;
    }

    return &tileData[x + y * w];
}

void Structure::copyFromMap(GameMap &map, Vector2 start, Vector2 end)
{
    if (end.x > map.w)
    {
        end.x = map.w - 1;
    }

    if (start.x > map.w)
    {
        start.x = map.w - 1;
    }

    if (start.y > map.h)
    {
        start.y = map.h - 1;
    }

    if (end.y > map.h)
    {
        end.y = map.h - 1;
    }

    Vector2 size = Vector2{end.x - start.x + 1, end.y - start.y + 1};

    if (size.x > map.w)
    {
        return;
    }

    if (size.y > map.h)
    {
        return;
    }

    create(size.x, size.y);

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            getBlocUnsafe(x, y) = map.getBlocUnsafe(x + start.x, y + start.y);
        }
    }
}

void Structure::pasteIntoMap(GameMap &map, Vector2 start)
{
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            auto b = map.getBlockSafe(x + start.x, y + start.y);

            if (b)
            {
                *b = getBlocUnsafe(x, y);
            }
        }
    }
}