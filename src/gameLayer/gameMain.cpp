#include <raylib.h>
#include "gameMain.h"
#include <assetManager.h>
#include <cmath>
#include <renderer.h>

AssetManager assetManager;

bool initGame()
{
    assetManager.loadAll();

    gameData.blockToPlace.type = Block::woodLog;

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
        gameData.blockToPlace.type = Block::woodLog;
    }

    if (IsKeyDown(KEY_TWO))
    {
        gameData.blockToPlace.type = Block::dirt;
    }

    if (IsKeyDown(KEY_THREE))
    {
        gameData.blockToPlace.type = Block::leaves;
    }

    if (IsKeyDown(KEY_FOUR))
    {
        gameData.blockToPlace.type = Block::copper;
    }

    Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
    int blockX = static_cast<int>(std::floor(worldPos.x));
    int blockY = static_cast<int>(std::floor(worldPos.y));

    // Destroy blocks
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Block *b = gameData.gameMap.getBlockSafe(blockX, blockY);
        if (b)
        {
            *b = {};
        }
    }

    // Place blocks
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Block *b = gameData.gameMap.getBlockSafe(blockX, blockY);
        if (b)
        {
            b->type = gameData.blockToPlace.type;
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