#include "worldGenerator.h"
#include <randomStuff.h>
#include <blocks.h>
#include <cmath>

void generateWorld(GameMap &gameMap, long seed)
{
    const int w = 900;
    const int h = 500;
    gameMap.create(w, h);
    std::ranlux24_base coordRNG(seed);

    // Ground starts at 1/4 of the way down the screen
    float baseHeight = h * 0.25f;

    for (int x = 0; x < w; x++)
    {
        // 1. FREQUENCY: If this is too small (e.g. 0.001), the hill is 2000px wide.
        // 2. AMPLITUDE: This is the height in pixels/tiles.

        // We use (x + (seed % 1000)) to ensure we aren't at the "flat" start of the sine wave
        auto xPos = static_cast<float>(x + (seed % 200));

        // Mountain: Wave every ~150 pixels
        float mountain = std::sin(xPos * 0.04f) * 20.0f;

        // Rolling Hills: Wave every ~40 pixels
        float hills = std::sin(xPos * 0.15f) * 12.0f;

        int surfaceY = static_cast<int>(baseHeight + mountain + hills);

        // Clamp to map bounds
        if (surfaceY < 0) surfaceY = 0;
        if (surfaceY >= h) surfaceY = h - 1;

        for (int y = 0; y < h; y++)
        {
            Block block;
            if (y < surfaceY)
            {
                block.type = Block::air;
            }
            else if (y == surfaceY)
            {
                block.type = Block::grassBlock;
            }
            else if (y < surfaceY + 8)
            {
                block.type = Block::dirt;
            }
            else
            {
                block.type = Block::stone;
                if (getRandomChance(coordRNG, 0.01f)) block.type = Block::gold;
            }

            gameMap.getBlocUnsafe(x, y) = block;
        }
    }
}