#include "gameMain.h"
#include "raylib.h"
#include <assetManager.h>
#include <gameMap.h>

struct GameData
{
    GameMap gameMap;
} gameData;

AssetManager assetManager;

bool initGame()
{
    assetManager.loadAll();

    gameData.gameMap.create(30, 10);

    gameData.gameMap.getBlocUnsafe(0, 0).type = Block::dirt;
    gameData.gameMap.getBlocUnsafe(1, 1).type = Block::dirt;
    gameData.gameMap.getBlocUnsafe(2, 2).type = Block::dirt;
    gameData.gameMap.getBlocUnsafe(3, 3).type = Block::dirt;
    gameData.gameMap.getBlocUnsafe(4, 4).type = Block::dirt;

    return true;
}

bool updateGame()
{
    float deltaTime = GetFrameTime();
    if (deltaTime > 1.5f / 5)
    {
        deltaTime = 1 / 5.f;
    }

    ClearBackground({75, 75, 150, 255});

    for (int y = 0; y < gameData.gameMap.h; y++)
    {
        for (int x = 0; x < gameData.gameMap.w; x++)
        {
            Block &block = gameData.gameMap.getBlocUnsafe(x, y);

            if (block.type != Block::air)
            {
                float size = 32;
                float posX = x * size;
                float posY = y * size;

                DrawTexturePro(assetManager.dirt, Rectangle{0.f, 0.f, static_cast<float>(assetManager.dirt.width), static_cast<float>(assetManager.dirt.height)}, // Sprite source
                    {posX, posY, size, size}, // Dest
                    {0, 0}, // Origin
                    0.0f, // rotation
                    WHITE); // tint
            }
        }
    }

    return true;
}

void closeGame()
{

}