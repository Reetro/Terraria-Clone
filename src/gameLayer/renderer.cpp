#include "renderer.h"
#include <helpers.h>
#include "raymath.h"
#include <blocks.h>
#include <gameMain.h>

void renderWoodLog(const AssetManager& assetManager, GameData& data, int x, int y)
{
    Block* up = data.gameMap.getBlockSafe(x, y - 1);
    Block* down = data.gameMap.getBlockSafe(x, y + 1);
    Block* left = data.gameMap.getBlockSafe(x - 1, y);
    Block* right = data.gameMap.getBlockSafe(x + 1, y);

    auto isLogOrLeaves = [](Block* b) {
        return b && (b->type == Block::woodLog || b->type == Block::leaves);
    };
    auto isLeaves = [](Block* b) {
        return b && b->type == Block::leaves;
    };
    auto isGround = [](Block* b) {
        return b && b->type != Block::woodLog && b->type != Block::leaves && b->type != Block::air;
    };

    bool hasLogOrLeavesAbove = isLogOrLeaves(up);
    bool hasLogOrLeavesBelow = isLogOrLeaves(down);
    bool hasLeavesLeft = isLeaves(left);
    bool hasLeavesRight = isLeaves(right);
    bool hasGroundBelow = isGround(down);

    int col = 0;

    // Log directly under leaves
    if (up && up->type == Block::leaves)
    {
        col = 5;
    }
    // Top log: no log or leaves above, log or leaves below
    else if (!hasLogOrLeavesAbove && hasLogOrLeavesBelow)
    {
        col = 6;
    }
    // Bottom of tree: solid ground directly below
    else if (hasGroundBelow)
    {
        col = 4;
    }
    // Single log: nothing above or below
    else if (!hasLogOrLeavesAbove && !hasLogOrLeavesBelow)
    {
        col = 7;
    }
    // Middle trunk with leaves on both sides
    else if (hasLeavesLeft && hasLeavesRight)
    {
        col = 1;
    }
    // Middle trunk with leaves on right only
    else if (hasLeavesRight)
    {
        col = 2;
    }
    // Middle trunk with leaves on left only
    else if (hasLeavesLeft)
    {
        col = 3;
    }
    // Plain middle trunk
    else
    {
        col = 0;
    }

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

void renderWalls(const AssetManager& assetManager, GameData& data, int x, int y)
{

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
            auto &[block] = data.gameMap.getBlocUnsafe(x, y);

            if (block == Block::woodLog)
            {
                renderWoodLog(assetManager, data, x, y);
                continue; // skip the normal draw
            }

            // Render normal blocks
            if (block != Block::air)
            {
                DrawTexturePro(
                    assetManager.textures,
                    getTextureAtlas(block, 0, 32, 32), //source
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