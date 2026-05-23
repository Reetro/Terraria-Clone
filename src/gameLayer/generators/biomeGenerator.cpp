#include "biomeGenerator.h"
#include <randomStuff.h>

int BiomeRegion::blockDirt() const
{
    switch (type)
    {
        case BiomeType::Desert:
            return Block::sand;
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
        default:
            return Block::stone;
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

    return biomes;
}