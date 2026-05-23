#include "biomeGenerator.h"
#include <randomStuff.h>

int BiomeRegion::blockDirt() const
{
    switch (type)
    {
        case BiomeType::Desert:
            return Block::sand;
        case BiomeType::Snow:
            return Block::snow;
        default:
            return Block::dirt;
    }
}

int BiomeRegion::blockGrass() const
{
    switch (type)
    {
        case BiomeType::Desert:
            return Block::sand;
        case BiomeType::Snow:
            return Block::snow;
        default:
            return Block::grassBlock;
    }
}

int BiomeRegion::blockStone() const
{
    switch (type)
    {
        case BiomeType::Desert:
            return Block::sandStone;
        case BiomeType::Snow:
            return Block::snowBricks;
        default:
            return Block::stone;
    }
}

int BiomeRegion::stoneDepth(int x, int stoneHeight) const
{
    switch (type)
    {
        case BiomeType::Desert:
        {
            int mid = (start + end) / 2;
            int halfWidth = (end - start) / 2;
            int distFromMid = std::abs(x - mid);

            float centeredness = 1.0f - distFromMid / float(halfWidth);

            int stoneStart = 10 + stoneHeight;
            int stoneDepth = 20 + stoneHeight;

            return stoneStart + int(centeredness * stoneDepth);
        }

        case BiomeType::Snow:
        {
            int mid = (start + end) / 2;
            int halfWidth = (end - start) / 2;
            int distFromMid = std::abs(x - mid);

            float centeredness = 1.0f - distFromMid / float(halfWidth);

            int stoneStart = 10 + stoneHeight;
            int stoneDepth = 20 + stoneHeight;

            return stoneStart + int(centeredness * stoneDepth);
        }
        default:
            return -1;
    }
}

std::vector<BiomeRegion> generateBiomes(int worldWidth, std::ranlux24_base& rng)
{
    std::vector<BiomeRegion> biomes;

    int desertStart = getRandomInt(rng, 10, worldWidth - 210);
    int desertEnd = desertStart + 100 + getRandomInt(rng, 0, 100);

    if (desertEnd > worldWidth)
    {
        desertEnd = worldWidth;
    }

    biomes.push_back({ desertStart, desertEnd, BiomeType::Desert });

    int snowStart = getRandomInt(rng, 10, worldWidth - 300);
    int snowEnd = snowStart + 100 + getRandomInt(rng, 0, 150);

    if (snowEnd > worldWidth)
    {
        snowEnd = worldWidth;
    }

    biomes.push_back({ snowStart, snowEnd, BiomeType::Snow });

    return biomes;
}