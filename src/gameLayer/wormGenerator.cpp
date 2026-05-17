#include "wormGenerator.h"
#include <blocks.h>
#include <cmath>
#include <algorithm>
#include <FastNoiseSIMD.h>

void carveTunnel(GameMap& map, int centerX, int centerY, double radius, const std::vector<int>& surfaceHeights)
{
    for (int dy = -(int)radius; dy <= (int)radius; ++dy)
    {
        for (int dx = -(int)radius; dx <= (int)radius; ++dx)
        {
            if (dx * dx + dy * dy <= radius * radius)
            {
                int targetX = centerX + dx;
                int targetY = centerY + dy;

                if (targetX >= 0 && targetY >= 0 && targetX < map.w && targetY < map.h)
                {
                    // Don't carve above the surface
                    if (targetY <= surfaceHeights[targetX])
                    {
                        continue;
                    }

                    map.getBlocUnsafe(targetX, targetY).type = Block::air;
                }
            }
        }
    }
}

void spawnCaveWorm(int minCount, int maxCount, GameMap& map, std::ranlux24_base& rng, long seed, const std::vector<int>& surfaceHeights)
{
    int numberOfWorms = getRandomInt(rng, minCount, maxCount);
    int mapW = map.w;
    int mapH = map.h;

    FastNoiseSIMD* fastNoise = FastNoiseSIMD::NewFastNoiseSIMD(seed++);
    fastNoise->SetNoiseType(FastNoiseSIMD::Simplex);
    fastNoise->SetFrequency(0.05f);

    float* noiseData = FastNoiseSIMD::GetEmptySet(mapW * mapH);
    fastNoise->FillNoiseSet(noiseData, 0, 0, 0, mapH, mapW, 1);

    auto sampleNoise = [&](int x, int y) -> float
    {
        x = std::clamp(x, 0, mapW - 1);
        y = std::clamp(y, 0, mapH - 1);
        return noiseData[x + y * mapW];
    };

    for (int w = 0; w < numberOfWorms; w++)
    {
        Worm worm{};
        worm.x = getRandomInt(rng, 2, mapW - 2);
        worm.y = getRandomInt(rng, 2, mapH - 2);
        worm.angle = static_cast<double>(getRandomInt(rng, 0, 10000)) / 10000.0 * 2.0 * M_PI;
        worm.length = getRandomInt(rng, 50, 200);
        worm.radius = 2.5 + static_cast<double>(getRandomInt(rng, 0, 1000)) / 1000.0 * 3.0;

        for (int step = 0; step < worm.length; ++step)
        {
            carveTunnel(map, worm.x, worm.y, worm.radius, surfaceHeights);

            worm.x += cos(worm.angle);
            worm.y += sin(worm.angle);

            if (worm.x < 2 || worm.x >= mapW - 2 || worm.y < 2 || worm.y >= mapH - 2)
            {
                break;
            }

            float noiseValue = sampleNoise((int)worm.x, (int)worm.y);
            double angleChange = noiseValue * (M_PI / 2.0);
            worm.angle += angleChange;
        }
    }

    FastNoiseSIMD::FreeNoiseSet(noiseData);
    delete fastNoise;
}