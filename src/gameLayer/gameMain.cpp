#include <raylib.h>
#include "gameMain.h"
#include <assetManager.h>
#include <cmath>
#include <iostream>
#include <ostream>
#include <renderer.h>

AssetManager assetManager;

bool initGame()
{
    assetManager.loadAll();

    gameData.blockToPlace.type = Block::woodLog;
    gameData.tileToPlace.type = Tile::dirtWall;

    gameData.gameMap.create(700, 500);

    for (int i = 0; i < 700; i++)
    {
        for (int j = 0; j < 500; j++)
        {
            gameData.gameMap.getBlocUnsafe(i, j).type = Block::stone;
        }
    }

    gameData.camera.target = {0, 0}; // World space view
    gameData.camera.rotation = 0.0f;
    gameData.camera.zoom = 100.0f;

    return true;
}

bool updateGame()
{
    float deltaTime = GetFrameTime();
    if (deltaTime > 1.5f / 5)
    {
        deltaTime = 1 / 5.f;
    }

    gameData.camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

    ClearBackground({75, 75, 150, 255});

#pragma region camera movement
    if (IsKeyDown(KEY_A))
    {
        gameData.camera.target.x -= 7.f * deltaTime;
    }

    if (IsKeyDown(KEY_D))
    {
        gameData.camera.target.x += 7.f * deltaTime;
    }

    if (IsKeyDown(KEY_W))
    {
        gameData.camera.target.y -= 7.f * deltaTime;
    }

    if (IsKeyDown(KEY_S))
    {
        gameData.camera.target.y += 7.f * deltaTime;
    }
#pragma endregion

#pragma region block placement
    if (IsKeyDown(KEY_ONE))
    {
        gameData.blockToPlace.type = -1;
        gameData.tileToPlace.type = Tile::sapling;
        gameData.isHoldingWall = true;
    }

    if (IsKeyDown(KEY_TWO))
    {
        gameData.blockToPlace.type = Block::dirt;
        gameData.isHoldingWall = false;
    }

    if (IsKeyDown(KEY_THREE))
    {
        gameData.blockToPlace.type = Block::leaves;
        gameData.isHoldingWall = false;
    }

    if (IsKeyDown(KEY_FOUR))
    {
        gameData.blockToPlace.type = Block::copper;
        gameData.isHoldingWall = false;
    }

    Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
    int blockX = static_cast<int>(std::floor(worldPos.x));
    int blockY = static_cast<int>(std::floor(worldPos.y));

    // Destroy blocks & tiles
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Block* b = gameData.gameMap.getBlockSafe(blockX, blockY);
        Tile* t = gameData.gameMap.getTileSafe(blockX, blockY);

        // 1. Check the foreground block first
        if (b && b->type != Block::air)
        {
            *b = {}; // Destroy block
        }
        // 2. If no block is there, try to destroy the wall
        else if (t && t->type != 0) // Assuming 0 is empty for your Tile enum
        {
            *t = {}; // Destroy wall
        }
    }

    // Place blocks
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if (gameData.isHoldingWall)
        {
            if (Tile* t = gameData.gameMap.getTileSafe(blockX, blockY))
            {
                t->type = gameData.tileToPlace.type;
            }
        }
        else
        {
            if (Block *b = gameData.gameMap.getBlockSafe(blockX, blockY); b != nullptr)
            {
                b->type = gameData.blockToPlace.type;
            }
        }
    }
#pragma endregion

    BeginMode2D(gameData.camera);

    renderWorld(assetManager, gameData);

    EndMode2D();

    DrawFPS(10, 10);

    return true;
}

void closeGame()
{

}