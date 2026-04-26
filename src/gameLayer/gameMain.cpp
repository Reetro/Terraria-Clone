#include <raylib.h>
#include "gameMain.h"
#include <assetManager.h>
#include <cmath>
#include <gameMap.h>
#include <helpers.h>

#include "raymath.h"

struct GameData
{
    GameMap gameMap;
    Camera2D camera{};
    Block blockToPlace;
} gameData;

AssetManager assetManager;

bool initGame()
{
    assetManager.loadAll();

    gameData.blockToPlace.type = Block::goldBlock;

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
        gameData.blockToPlace.type = Block::goldBlock;
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

#pragma region Draw world (Only draw visable tiles)
    BeginMode2D(gameData.camera);

    Vector2 topLeftView = GetScreenToWorld2D({0, 0}, gameData.camera);
    Vector2 bottomRightView = GetScreenToWorld2D({static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, gameData.camera);

    int startXView = (int)floorf(topLeftView.x - 1);
    int endXView = (int)ceilf(bottomRightView.x + 1);
    int startYView = (int)floorf(topLeftView.y - 1);
    int endYView = (int)ceilf(bottomRightView.y + 1);

    startXView = Clamp(startXView, 0, gameData.gameMap.w - 1);
    endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);

    startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
    endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);


    for (int y = startYView; y <= endYView; y++)
    {
        for (int x = startXView; x <= endXView; x++)
        {

            auto &b = gameData.gameMap.getBlocUnsafe(x, y);

            if (b.type != Block::air)
            {

                DrawTexturePro(
                    assetManager.textures,
                    getTextureAtlas(b.type, 0, 32, 32), //source
                    {(float)x, (float)y, 1, 1}, //dest
                    {0, 0},// origin (top-left corner)
                    0.0f, // rotation
                    WHITE // tint
                );

            }
        }
    }
#pragma endregion

    //draw selected block
    DrawTexturePro(
        assetManager.frame,
        {0,0, static_cast<float>(assetManager.frame.width), static_cast<float>(assetManager.frame.height)}, //source
        {static_cast<float>(blockX), static_cast<float>(blockY), 1, 1}, //dest
        {0, 0},// origin (top-left corner)
        0.0f, // rotation
        WHITE // tint
    );

    EndMode2D();

    DrawFPS(10, 10);

    return true;
}

void closeGame()
{

}