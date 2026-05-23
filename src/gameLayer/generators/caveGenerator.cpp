#include "caveGenerator.h"
#include <blocks.h>
#include <FastNoiseSIMD.h>
#include <memory>

void carveCaves(GameMap& gameMap, int w, int h, long& seed, const CaveConfig& config)
{
    std::unique_ptr<FastNoiseSIMD> cavesNoise(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> caves2Noise(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> caves3Noise(FastNoiseSIMD::NewFastNoiseSIMD());

    cavesNoise->SetSeed(seed++);
    caves2Noise->SetSeed(seed++);
    caves3Noise->SetSeed(seed++);

    cavesNoise->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    cavesNoise->SetFractalOctaves(2);
    cavesNoise->SetFrequency(0.02f);

    caves2Noise->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    caves2Noise->SetFractalOctaves(3);
    caves2Noise->SetFrequency(0.04f);

    caves3Noise->SetNoiseType(FastNoiseSIMD::NoiseType::Simplex);
    caves3Noise->SetFrequency(0.005f);

    float* caveNoise1 = FastNoiseSIMD::GetEmptySet(w * h);
    float* caveNoise2 = FastNoiseSIMD::GetEmptySet(w * h);
    float* selectorNoise = FastNoiseSIMD::GetEmptySet(w * h);

    cavesNoise->FillNoiseSet(caveNoise1, 0, 0, 0, h, w, 1);
    caves2Noise->FillNoiseSet(caveNoise2, 0, 0, 0, h, w, 1);
    caves3Noise->FillNoiseSet(selectorNoise, 0, 0, 0, h, w, 1);

    for (int i = 0; i < w * h; i++)
    {
        caveNoise1[i] = (caveNoise1[i] + 1) / 2;
        caveNoise2[i] = (caveNoise2[i] + 1) / 2;
        selectorNoise[i] = (selectorNoise[i] + 1) / 2;
    }

    auto getIdx = [w](int x, int y) { return x + y * w; };

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            int i = getIdx(x, y);

            float caveA = caveNoise1[i];
            float caveB = caveNoise2[i];
            float selector = selectorNoise[i];
            float blended = caveA + selector * (caveB - caveA);

            if (blended < config.threshold)
            {
                gameMap.getBlocUnsafe(x, y).type = Block::air;
            }
        }
    }

    FastNoiseSIMD::FreeNoiseSet(caveNoise1);
    FastNoiseSIMD::FreeNoiseSet(caveNoise2);
    FastNoiseSIMD::FreeNoiseSet(selectorNoise);
}