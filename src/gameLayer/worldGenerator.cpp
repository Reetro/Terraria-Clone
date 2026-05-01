#include "worldGenerator.h"
#include <blocks.h>
#include <FastNoiseSIMD.h>
#include <memory>
#include <randomStuff.h>

void generateWorld(GameMap &gameMap, long seed)
{
    const int w = 900;
    const int h = 500;
    gameMap.create(w, h);

    std::ranlux24_base rng(seed++);

    int desertStart = getRandomInt(rng, 10, w -210);
    int desertEnd = desertStart + 100 + getRandomInt(rng, 0, 100);
    if (desertEnd > w)
    {
        desertEnd = w;
    }

    std::unique_ptr<FastNoiseSIMD> dirtNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> stoneNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());

    dirtNoiseGenerator->SetSeed(seed++);
    stoneNoiseGenerator->SetSeed(seed++);

    dirtNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    dirtNoiseGenerator->SetFractalOctaves(1);
    dirtNoiseGenerator->SetFrequency(0.01);

    stoneNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    stoneNoiseGenerator->SetFractalOctaves(2);
    stoneNoiseGenerator->SetFrequency(0.02);

    float *dirtNoise = FastNoiseSIMD::GetEmptySet(w);
    float *stoneNoise = FastNoiseSIMD::GetEmptySet(w);

    dirtNoiseGenerator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);
    stoneNoiseGenerator->FillNoiseSet(stoneNoise, 0, 0, 0, w, 1, 1);

    // Convert from [-1, 1] to [0, 1]
    for (int i = 0; i < w; i++)
    {
        dirtNoise[i] = (dirtNoise[i] + 2) / 2;
        stoneNoise[i] = (stoneNoise[i] + 2) / 4;

        stoneNoise[i] = std::pow(stoneNoise[i], 2); // steeper mountains
    }

    int dirtOffsetStart = -5;
    int dirtOffsetEnd = 35;

    int stoneHeightStart = 10;
    int stoneHeightEnd = 170;

    for (int x = 0; x < w; x++)
    {
        int stoneHeight = stoneHeightStart + (stoneHeightEnd - stoneHeightStart) * stoneNoise[x];
        int dirtHeight = dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];
        bool inDesert = (x >= desertStart && x <= desertEnd);

        int dirtType = Block::dirt;
        int grassType = Block::grassBlock;
        int stoneType = Block::stone;

        if (inDesert)
        {
            dirtType = Block::sand;
            grassType = Block::sand;
            stoneType = Block::sandStone;
        }

        for (int y = 0; y < h; y++)
        {
            Block block;

            if (y > dirtHeight)
            {
                block.type = dirtType;

                if (getRandomChance(rng, 0.01f))
                {
                    block.type = stoneType;
                }
            }

            if (y == dirtHeight)
            {
                block.type = grassType;

                if (getRandomChance(rng, 0.01f))
                {
                    block.type = stoneType;
                }
            }

            if (y >= stoneHeight || (getRandomChance(rng, 0.02f) && y > dirtHeight))
            {
                block.type = Block::stone;

                if (getRandomChance(rng, 0.05f))
                {
                    block.type = dirtType;
                }
            }

            if (inDesert)
            {
                int desertMid = (desertStart + desertEnd) / 2;
                int desertHalfWidth = (desertEnd - desertStart) / 2;
                int distanceFromDesertMid = std::abs(x - desertMid);

                // This gives a value from 0 at edge to 1 at center
                float desertDistance = 1 - distanceFromDesertMid / float(desertHalfWidth);

                int desertStoneStart = 10 + stoneHeight;
                int desertStoneDepth = 20 + stoneHeight; // how deep the triangle goes

                int triangleStoneY = desertStoneStart + desertDistance * desertStoneDepth;

                // Apply stone if below the triangle
                if (y > triangleStoneY)
                {
                    block.type = Block::stone;
                }
            }

            gameMap.getBlocUnsafe(x, y) = block;
        }
    }

    FastNoiseSIMD::FreeNoiseSet(dirtNoise);
    FastNoiseSIMD::FreeNoiseSet(stoneNoise);
}
