#include "renderer.h"
#include <helpers.h>
#include "raymath.h"
#include <blocks.h>
#include <gameMain.h>
#include <iostream>
#include <randomStuff.h>

int getTreeColumn(Block* up, Block* down, Block* left, Block* right)
{
    auto isLogOrLeaves = [](Block* b) {
        return b && (b->type == Block::woodLog || b->type == Block::leaves);
    };
    auto isLeaves = [](Block* b) {
        return b && b->type == Block::leaves;
    };
    auto isGround = [](Block* b) {
        return b && b->type != Block::woodLog && b->type != Block::leaves && b->type != Block::air;
    };

    if (up && up->type == Block::leaves)
    {
        return 5;
    }
    if (!isLogOrLeaves(up) && isLogOrLeaves(down))
    {
        return 6;
    }
    if (isGround(down))
    {
        return 4;
    }
    if (!isLogOrLeaves(up) && !isLogOrLeaves(down))
    {
        return 7;
    }
    if (isLeaves(left) && isLeaves(right))
    {
        return 1;
    }
    if (isLeaves(right))
    {
        return 2;
    }
    if (isLeaves(left))
    {
        return 3;
    }

    return 0;
}


void renderWoodLog(const AssetManager& assetManager, GameData& data, const int x, const int y)
{
    // Get the blocks around the current block
    Block* up = data.gameMap.getBlockSafe(x, y - 1);
    Block* down = data.gameMap.getBlockSafe(x, y + 1);
    Block* left = data.gameMap.getBlockSafe(x - 1, y);
    Block* right = data.gameMap.getBlockSafe(x + 1, y);

    int col = getTreeColumn(up, down, left, right);
    int row = (x * 7 + y * 3) % 4;

    DrawTexturePro(
        assetManager.tree,
        getTreeAtlas(col, row),
        {static_cast<float>(x), static_cast<float>(y), 1, 1},
        {0, 0},
        0.0f,
        WHITE
    );
}

void renderTile(const AssetManager& assetManager, GameData& data, int x, int y)
{
    auto &[tile] = data.gameMap.getTileUnsafe(x, y);

    DrawTexturePro(
        assetManager.tiles,
        getTextureAtlas(tile, 4, 32, 32), //source
        {static_cast<float>(x), static_cast<float>(y), 1, 1}, //dest
        {0, 0},// origin (top-left corner)
        0.0f, // rotation
        WHITE // tint
    );
}

void drawSelectedBlock(const AssetManager& assetManager)
{
    auto [x, y] = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
    const int blockX = static_cast<int>(std::floor(x));
    const int blockY = static_cast<int>(std::floor(y));

    DrawTexturePro(
        assetManager.frame,
        {0,0, static_cast<float>(assetManager.frame.width), static_cast<float>(assetManager.frame.height)}, //source
        {static_cast<float>(blockX), static_cast<float>(blockY), 1, 1}, //dest
        {0, 0},// origin (top-left corner)
        0.0f, // rotation
        WHITE // tint
    );
}

void renderWorld(const AssetManager& assetManager, GameData& data)
{
    // Calculate what blocks are in the current view
    Vector2 topLeftView = GetScreenToWorld2D({0, 0}, data.camera);
    Vector2 bottomRightView = GetScreenToWorld2D({static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, data.camera);

    int startXView = static_cast<int>(floorf(topLeftView.x - 1));
    int endXView = static_cast<int>(ceilf(bottomRightView.x + 1));
    int startYView = static_cast<int>(floorf(topLeftView.y - 1));
    int endYView = static_cast<int>(ceilf(bottomRightView.y + 1));

    startXView = Clamp(startXView, 0, data.gameMap.w - 1);
    endXView = Clamp(endXView, 0, data.gameMap.w - 1);

    startYView = Clamp(startYView, 0, data.gameMap.h - 1);
    endYView = Clamp(endYView, 0, data.gameMap.h - 1);

    // Draw all blocks that are currently in view of the camera
    for (int y = startYView; y <= endYView; y++)
    {
        for (int x = startXView; x <= endXView; x++)
        {
            renderTile(assetManager, data, x, y);

            auto &[block] = data.gameMap.getBlocUnsafe(x, y);

            if (block == Block::woodLog)
            {
                renderWoodLog(assetManager, data, x, y);
                continue; // skip the normal draw
            }

            // Render normal blocks
            if (block != Block::air)
            {
                // Assign blocks random textures

                // 1. Create a unique seed for this specific coordinate
                // Using primes (73856093, 19349663) helps distribute the seed values
                unsigned int coordinateSeed = static_cast<unsigned int>(x) * 73856093 ^ static_cast<unsigned int>(y) * 19349663;

                // 2. Initialize your engine with that seed
                std::ranlux24_base rng(coordinateSeed);

                int variantY = getRandomInt(rng, 0, 3);

                DrawTexturePro(
                    assetManager.textures,
                    getTextureAtlas(block, variantY, 32, 32), //source
                    {static_cast<float>(x), static_cast<float>(y), 1, 1}, //dest
                    {0, 0},// origin (top-left corner)
                    0.0f, // rotation
                    WHITE // tint
                );
            }
        }
    }

    drawSelectedBlock(assetManager);
}