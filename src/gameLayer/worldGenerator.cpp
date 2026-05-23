#include "worldGenerator.h"
#include <random>
#include <structure.h>
#include "saveMap.h"
#include "generators/biomeGenerator.h"
#include "generators/caveGenerator.h"
#include "generators/structureSpawner.h"
#include "generators/terrainGenerator.h"
#include "generators/wormGenerator.h"

void generateWorld(GameMap& gameMap, long seed)
{
    const int w = 900;
    const int h = 500;
    gameMap.create(w, h);

    std::ranlux24_base rng(seed++);

    // Biomes
    std::vector<BiomeRegion> biomes = generateBiomes(w, rng);

    // Terrain noise + block placement
    TerrainNoiseArrays terrainNoise = generateTerrainNoise(w, seed);
    generateTerrain(gameMap, w, h, terrainNoise, biomes, rng);

    // Caves (carves into existing terrain)
    carveCaves(gameMap, w, h, seed);

    // Surface heights for worm spawning
    std::vector<int> surfaceHeights = buildSurfaceHeights(w, terrainNoise);
    spawnCaveWorm(20, 120, gameMap, rng, seed, surfaceHeights);

    // Structures
    Structure treeStructure;
    loadBlockDataFromFile(treeStructure.mapData, treeStructure.w, treeStructure.h, RESOURCES_PATH "structures/tree.bin");
    spawnTrees(gameMap, treeStructure, w, h, rng);
}
