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


void renderWoodLog(AssetManager& assetManager, const int x, const int y)
{
    // Get the blocks around the current block
    Block* up = gameData.gameMap.getBlockSafe(x, y - 1);
    Block* down = gameData.gameMap.getBlockSafe(x, y + 1);
    Block* left = gameData.gameMap.getBlockSafe(x - 1, y);
    Block* right = gameData.gameMap.getBlockSafe(x + 1, y);

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

void renderTile(AssetManager& assetManager, int x, int y)
{
    auto &[tile] = gameData.gameMap.getTileUnsafe(x, y);

    DrawTexturePro(
        assetManager.tiles,
        getTextureAtlas(tile, 4, 32, 32), //source
        {static_cast<float>(x), static_cast<float>(y), 1, 1}, //dest
        {0, 0},// origin (top-left corner)
        0.0f, // rotation
        WHITE // tint
    );
}

void renderPlayer(AssetManager& assetManager)
{
    Transform2D playerSprite = gameData.player.transform;
    playerSprite.w = 1;
    playerSprite.h = 2;
    //move the sprite so that the bottom of the sprite matches the bottom of the collider
    playerSprite.pos.y -= (playerSprite.h - gameData.player.transform.h) / 2;

    DrawTexturePro(
        assetManager.player,
        {0, 0, static_cast<float>(assetManager.player.width), static_cast<float>(assetManager.player.height)},
        playerSprite.getAABB(), //dest
        {0, 0},// origin (top-left corner)
        0.0f, // rotation
        WHITE // tint
    );

    DrawRectangleLinesEx(gameData.player.transform.getAABB(), 0.1,
        {20, 101, 250, 120});
}

void renderEntities(AssetManager& assetManager)
{
    for (auto &e : gameData.entities.entities)
    {
        e.second->render(assetManager);
    }
}

void drawSelectedBlock(const AssetManager& assetManager)
{
    auto [x, y] = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
    const int blockX = static_cast<int>(std::floor(x));
    const int blockY = static_cast<int>(std::floor(y));

    int itemToShow = 0;
    if (gameData.isHoldingTile)
    {
        itemToShow = gameData.creativeSelectedTile;
    }
    else
    {
        itemToShow = gameData.creativeSelectedBlock;
    }

    DrawTexturePro(
        assetManager.textures,
        getTextureAtlas(itemToShow, 4, 32, 32), //source
{static_cast<float>(blockX), static_cast<float>(blockY), 1, 1},
        {0, 0},// origin (top-left corner)
        0.0f, // rotation
        {255, 255, 255, 200} // tint
    );
}

void renderWorld(AssetManager& assetManager)
{
    // Calculate what blocks are in the current view
    Vector2 topLeftView = GetScreenToWorld2D({0, 0}, gameData.camera);
    Vector2 bottomRightView = GetScreenToWorld2D({static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, gameData.camera);

    int startXView = static_cast<int>(floorf(topLeftView.x - 1));
    int endXView = static_cast<int>(ceilf(bottomRightView.x + 1));
    int startYView = static_cast<int>(floorf(topLeftView.y - 1));
    int endYView = static_cast<int>(ceilf(bottomRightView.y + 1));

    startXView = Clamp(startXView, 0, gameData.gameMap.w - 1);
    endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);

    startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
    endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);

    // Draw all blocks that are currently in view of the camera
    for (int y = startYView; y <= endYView; y++)
    {
        for (int x = startXView; x <= endXView; x++)
        {
            renderTile(assetManager, x, y);

            auto &[block] = gameData.gameMap.getBlocUnsafe(x, y);

            if (block == Block::woodLog)
            {
                renderWoodLog(assetManager, x, y);
                continue; // skip the normal draw
            }

            // Render normal blocks
            if (block != Block::air)
            {
                // Assign blocks random textures this is a really terrible way to do this but am lazy right now

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
    renderEntities(assetManager);
    renderPlayer(assetManager);
}