#include "terrainGenerator.h"
#include <blocks.h>
#include <FastNoiseSIMD.h>
#include <memory>
#include <cmath>

TerrainNoiseArrays::~TerrainNoiseArrays()
{
    FastNoiseSIMD::FreeNoiseSet(dirtNoise);
    FastNoiseSIMD::FreeNoiseSet(stoneNoise);
}

TerrainNoiseArrays generateTerrainNoise(int w, long &seed)
{
    TerrainNoiseArrays arrays;
    arrays.width = w;

    std::unique_ptr<FastNoiseSIMD> dirtGen(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> stoneGen(FastNoiseSIMD::NewFastNoiseSIMD());

    dirtGen->SetSeed(seed++);
    stoneGen->SetSeed(seed++);

    dirtGen->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    dirtGen->SetFractalOctaves(2);
    dirtGen->SetFrequency(0.01f);

    stoneGen->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    stoneGen->SetFractalOctaves(4);
    stoneGen->SetFrequency(0.01f);

    arrays.dirtNoise = FastNoiseSIMD::GetEmptySet(w);
    arrays.stoneNoise = FastNoiseSIMD::GetEmptySet(w);

    dirtGen->FillNoiseSet(arrays.dirtNoise, 0, 0, 0, w, 1, 1);
    stoneGen->FillNoiseSet(arrays.stoneNoise, 0, 0, 0, w, 1, 1);

    for (int i = 0; i < w; i++)
    {
        arrays.dirtNoise[i] = (arrays.dirtNoise[i] + 2) / 2;
        arrays.stoneNoise[i] = (arrays.stoneNoise[i] + 2) / 4;
        arrays.stoneNoise[i] = std::pow(arrays.stoneNoise[i], 2);
    }

    return arrays;
}

static const BiomeRegion *findBiome(const std::vector<BiomeRegion> &biomes, int x)
{
    for (const auto &b: biomes)
    {
        if (b.contains(x))
        {
            return &b;
        }
    }

    return nullptr;
}

static void applyDesertStone(
    Block &block,
    const BiomeRegion &biome,
    int x,
    int y,
    int stoneHeight
)
{
    int desertMid = (biome.start + biome.end) / 2;
    int desertHalfWidth = (biome.end - biome.start) / 2;
    int distFromMid = std::abs(x - desertMid);

    float desertDistance = 1.0f - distFromMid / float(desertHalfWidth);

    int desertStoneStart = 10 + stoneHeight;
    int desertStoneDepth = 20 + stoneHeight;
    int triangleStoneY = desertStoneStart + int(desertDistance * desertStoneDepth);

    if (y > triangleStoneY)
    {
        block.type = Block::stone;
    }
}

void generateTerrain(
    GameMap &gameMap,
    int w,
    int h,
    const TerrainNoiseArrays &noise,
    const std::vector<BiomeRegion> &biomes,
    std::ranlux24_base &rng,
    const TerrainConfig &config
)
{
    for (int x = 0; x < w; x++)
    {
        int stoneHeight = config.stoneHeightStart
                          + int((config.stoneHeightEnd - config.stoneHeightStart) * noise.stoneNoise[x]);

        int dirtHeight = config.dirtOffsetStart
                         + int((config.dirtOffsetEnd - config.dirtOffsetStart) * noise.dirtNoise[x]);

        const BiomeRegion *biome = findBiome(biomes, x);

        int dirtType = biome ? biome->blockDirt() : Block::dirt;
        int grassType = biome ? biome->blockGrass() : Block::grassBlock;
        int stoneType = biome ? biome->blockStone() : Block::stone;

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

            if (biome && biome->type == BiomeType::Desert)
            {
                applyDesertStone(block, *biome, x, y, stoneHeight);
            }

            gameMap.getBlocUnsafe(x, y) = block;
        }
    }
}

std::vector<int> buildSurfaceHeights(int w, const TerrainNoiseArrays &noise, const TerrainConfig &config)
{
    std::vector<int> heights(w);

    for (int x = 0; x < w; x++)
    {
        heights[x] = config.dirtOffsetStart
                     + int((config.dirtOffsetEnd - config.dirtOffsetStart) * noise.dirtNoise[x]);
    }

    return heights;
}