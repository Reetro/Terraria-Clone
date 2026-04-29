#include <raylib.h>
#include "gameMain.h"
#include <assetManager.h>
#include <chrono>
#include <cmath>
#include <imgui.h>
#include <renderer.h>
#include <worldGenerator.h>
#include <randomStuff.h>

AssetManager assetManager;

bool initGame()
{
    assetManager.loadAll();

    gameData.blockToPlace.type = Block::woodLog;
    gameData.tileToPlace.type = Tile::dirtWall;

    long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    generateWorld(gameData.gameMap, seed);

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
    static float CAMERA_SPEED = 10;

    if (IsKeyDown(KEY_A))
    {
        gameData.camera.target.x -= CAMERA_SPEED * deltaTime;
    }

    if (IsKeyDown(KEY_D))
    {
        gameData.camera.target.x += CAMERA_SPEED * deltaTime;
    }

    if (IsKeyDown(KEY_W))
    {
        gameData.camera.target.y -= CAMERA_SPEED * deltaTime;
    }

    if (IsKeyDown(KEY_S))
    {
        gameData.camera.target.y += CAMERA_SPEED * deltaTime;
    }
#pragma endregion

#pragma region block placement
    if (IsKeyDown(KEY_ONE))
    {
        gameData.blockToPlace.type = -1;
        gameData.tileToPlace.type = Tile::stoneWall;
        gameData.isHoldingTile = true;
    }

    if (IsKeyDown(KEY_TWO))
    {
        gameData.blockToPlace.type = Block::dirt;
        gameData.isHoldingTile = false;
    }

    if (IsKeyDown(KEY_THREE))
    {
        gameData.blockToPlace.type = Block::leaves;
        gameData.isHoldingTile = false;
    }

    if (IsKeyDown(KEY_FOUR))
    {
        gameData.blockToPlace.type = Block::copper;
        gameData.isHoldingTile = false;
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
        if (gameData.isHoldingTile)
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

    ImGui::Begin("Game Control");

    ImGui::SliderFloat("Camera Zoom:", &gameData.camera.zoom, 10.0f, 150);
    ImGui::SliderFloat("Camera Speed:", &CAMERA_SPEED, 5, 30);

    ImGui::End();

    DrawFPS(10, 10);

    return true;
}

void closeGame()
{

}