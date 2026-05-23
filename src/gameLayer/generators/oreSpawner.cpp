#include "oreSpawner.h"
#include "randomStuff.h"

void spawnOres(GameMap& gameMap, int w, int h, const std::vector<OreSettings>& ores, std::ranlux24_base& rng)
{
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            if (gameMap.getBlocUnsafe(x, y).type != Block::stone)
            {
                continue;
            }

            for (const OreSettings& ore : ores)
            {
                if (y < ore.minDepth)
                {
                    continue;
                }

                if (getRandomChance(rng, ore.spawnChance))
                {
                    gameMap.getBlocUnsafe(x, y).type = ore.blockType;
                    break;
                }
            }
        }
    }
}
