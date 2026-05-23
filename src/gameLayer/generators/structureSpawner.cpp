#include "structureSpawner.h"
#include <blocks.h>

void spawnTrees(GameMap& gameMap, Structure treeStructure, int w, int h, std::ranlux24_base rng)
{
    for (int x = 0; x < w; x++)
    {
        if (!getRandomChance(rng, 0.6f))
        {
            continue;
        }

        for (int y = 0; y < h; y++)
        {
            if (gameMap.getBlocUnsafe(x, y).type != Block::grassBlock)
            {
                continue;
            }

            int trunkCol = x + 2;
            int pasteY   = y - treeStructure.h;

            auto below = gameMap.getBlockSafe(trunkCol, y);
            bool validPlacement = (trunkCol < w) && below && (below->type == Block::grassBlock);

            if (validPlacement)
            {
                Vector2 spawnPos
                {
                    static_cast<float>(x),
                    static_cast<float>(pasteY)
                };

                treeStructure.pasteIntoMap(gameMap, spawnPos);
                x += treeStructure.w;
            }

            break;
        }
    }
}