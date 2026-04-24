#include "gameMain.h"
#include "raylib.h"
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>

struct GameData
{
    GameMap gameMap;
    Camera2D camera;
} gameData;

AssetManager assetManager;

bool initGame()
{
    assetManager.loadAll();

    gameData.gameMap.create(30, 10);

    gameData.gameMap.getBlocUnsafe(0, 0).type = Block::dirt;
    gameData.gameMap.getBlocUnsafe(1, 1).type = Block::grass;
    gameData.gameMap.getBlocUnsafe(2, 2).type = Block::goldBlock;
    gameData.gameMap.getBlocUnsafe(3, 3).type = Block::glass;
    gameData.gameMap.getBlocUnsafe(4, 4).type = Block::platform;

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

    BeginMode2D(gameData.camera);

    for (int y = 0; y < gameData.gameMap.h; y++)
    {
        for (int x = 0; x < gameData.gameMap.w; x++)
        {
            Block &block = gameData.gameMap.getBlocUnsafe(x, y);

            if (block.type != Block::air)
            {
                DrawTexturePro(assetManager.textures, getTextureAtlas(block.type, 0, 32, 32), {static_cast<float>(x), static_cast<float>(y), 1, 1}, {0, 0}, 0.0f, WHITE);
            }
        }
    }

    EndMode2D();

    return true;
}

void closeGame()
{

}