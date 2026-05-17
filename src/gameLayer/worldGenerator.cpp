#include "worldGenerator.h"
#include <blocks.h>
#include <FastNoiseSIMD.h>
#include <memory>
#include <randomStuff.h>
#include <wormGenerator.h>

void generateWorld(GameMap &gameMap, long seed)
{
    const int w = 900;
    const int h = 500;
    gameMap.create(w, h);

    std::ranlux24_base rng(seed++);

    int desertStart = getRandomInt(rng, 10, w - 210);
    int desertEnd = desertStart + 100 + getRandomInt(rng, 0, 100);
    if (desertEnd > w)
    {
        desertEnd = w;
    }

    std::unique_ptr<FastNoiseSIMD> dirtNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> stoneNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> cavesNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> caves2NoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> caves3NoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());

    dirtNoiseGenerator->SetSeed(seed++);
    stoneNoiseGenerator->SetSeed(seed++);
    cavesNoiseGenerator->SetSeed(seed++);
    caves2NoiseGenerator->SetSeed(seed++);
    caves3NoiseGenerator->SetSeed(seed++);

    dirtNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    dirtNoiseGenerator->SetFractalOctaves(2);
    dirtNoiseGenerator->SetFrequency(0.01);

    stoneNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    stoneNoiseGenerator->SetFractalOctaves(4);
    stoneNoiseGenerator->SetFrequency(0.01);

    cavesNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    cavesNoiseGenerator->SetFractalOctaves(2);
    cavesNoiseGenerator->SetFrequency(0.02);

    caves2NoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    caves2NoiseGenerator->SetFractalOctaves(3); // different character
    caves2NoiseGenerator->SetFrequency(0.04f); // higher freq = smaller features

    caves3NoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::Simplex); // non-fractal, smoother regions
    caves3NoiseGenerator->SetFrequency(0.005f); // very low freq = large blobs of each style

    float *dirtNoise = FastNoiseSIMD::GetEmptySet(w);
    float *stoneNoise = FastNoiseSIMD::GetEmptySet(w);
    float *cavesNoise = FastNoiseSIMD::GetEmptySet(w * h);
    float *caves2Noise = FastNoiseSIMD::GetEmptySet(w * h);
    float *caves3Noise = FastNoiseSIMD::GetEmptySet(w * h);

    dirtNoiseGenerator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);
    stoneNoiseGenerator->FillNoiseSet(stoneNoise, 0, 0, 0, w, 1, 1);
    cavesNoiseGenerator->FillNoiseSet(cavesNoise, 0, 0, 0, h, w, 1); //make sure you flip w and h!
    caves2NoiseGenerator->FillNoiseSet(caves2Noise, 0, 0, 0, h, w, 1);
    caves3NoiseGenerator->FillNoiseSet(caves3Noise, 0, 0, 0, h, w, 1);

    // Convert from [-1, 1] to [0, 1]
    for (int i = 0; i < w; i++)
    {
        dirtNoise[i] = (dirtNoise[i] + 2) / 2;
        stoneNoise[i] = (stoneNoise[i] + 2) / 4;

        stoneNoise[i] = std::pow(stoneNoise[i], 2); // steeper mountains
    }

    //convert from [-1 1] to [0 1]
    for (int i = 0; i < w * h; i++)
    {
        cavesNoise[i] = (cavesNoise[i] + 1) / 2;
        caves2Noise[i] = (caves2Noise[i] + 1) / 2;
        caves3Noise[i] = (caves3Noise[i] + 1) / 2;
    }

    auto getCaveNoise = [&](int x, int y)
    {
        return cavesNoise[x + y * w];
    };

    auto getCave2Noise = [&](int x, int y)
    {
        return caves2Noise[x + y * w];
    };

    auto getCave3Noise = [&](int x, int y)
    {
        return caves3Noise[x + y * w];
    };

    int dirtOffsetStart = -5;
    int dirtOffsetEnd = 35;

    int stoneHeightStart = 50;
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

            //bigger more interesting caves
            //getCaveNoise(x, y) < 0.80 && getCaveNoise(x, y) > 0.60

            float caveA = getCaveNoise(x, y);
            float caveB = getCave2Noise(x, y);
            float selector = getCave3Noise(x, y); // 0..1, large smooth regions

            float caveFinal = caveA + selector * (caveB - caveA);

            if (caveFinal < 0.35f)
            {
                block.type = Block::air;
            }

            gameMap.getBlocUnsafe(x, y) = block;
        }
    }

    // Build surface height array after the terrain loop
    std::vector<int> surfaceHeights(w);

    for (int x = 0; x < w; x++)
    {
        int dirtHeight = dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];
        surfaceHeights[x] = dirtHeight;
    }

    spawnCaveWorm(20, 120, gameMap, rng, seed, surfaceHeights);

    FastNoiseSIMD::FreeNoiseSet(dirtNoise);
    FastNoiseSIMD::FreeNoiseSet(stoneNoise);
    FastNoiseSIMD::FreeNoiseSet(cavesNoise);
    FastNoiseSIMD::FreeNoiseSet(caves2Noise);
    FastNoiseSIMD::FreeNoiseSet(caves3Noise);
}
