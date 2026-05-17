#include <raylib.h>
#include "gameMain.h"
#include <assetManager.h>
#include <chrono>
#include <cmath>
#include <imgui.h>
#include <renderer.h>
#include <worldGenerator.h>
#include <randomStuff.h>

#include "helpers.h"

AssetManager assetManager;

bool showImgui = false;

bool initGame()
{
    assetManager.loadAll();

    long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    generateWorld(gameData.gameMap, seed);

    gameData.camera.target = {20, 120}; // World space view
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

    if (IsKeyPressed(KEY_F10))
    {
        showImgui = !showImgui;
    }

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
    Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
    int blockX = static_cast<int>(std::floor(worldPos.x));
    int blockY = static_cast<int>(std::floor(worldPos.y));

    if (gameData.creativeSelectedBlock < 0)
    {
        gameData.creativeSelectedBlock = 0;
    }

    if (gameData.creativeSelectedBlock >= Block::BLOCKS_COUNT)
    {
        gameData.creativeSelectedBlock = Block::BLOCKS_COUNT - 1;
    }

    if (!showImgui)
    {
        // Destroy blocks & tiles
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Block *b = gameData.gameMap.getBlockSafe(blockX, blockY);
            Tile *t = gameData.gameMap.getTileSafe(blockX, blockY);

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
                if (Tile *t = gameData.gameMap.getTileSafe(blockX, blockY))
                {
                    t->type = gameData.creativeSelectedTile;
                }
            }
            else
            {
                if (Block *b = gameData.gameMap.getBlockSafe(blockX, blockY); b != nullptr)
                {
                    b->type = gameData.creativeSelectedBlock;
                }
            }
        }
    }

#pragma endregion

    BeginMode2D(gameData.camera);

    renderWorld(assetManager, gameData);

    EndMode2D();

    if (showImgui)
    {
        ImGui::Begin("Game Control");

        ImGui::SliderFloat("Camera Zoom:", &gameData.camera.zoom, 10.0f, 150);
        ImGui::SliderFloat("Camera Speed:", &CAMERA_SPEED, 5, 30);
        ImGui::Text("FPS: %d", GetFPS());

        ImGui::Separator();

        ImGui::Text("Blocks");

        ImGui::Separator();

        // Creative menu
        for (int i = 0; i < Block::BLOCKS_COUNT; i++)
        {
            Rectangle atlas = getTextureAtlas(i, 0, 32, 32);

            atlas.x /= assetManager.textures.width;
            atlas.width /= assetManager.textures.width;
            atlas.height /= assetManager.textures.height;
            atlas.y /= assetManager.textures.height;

            ImGui::PushID(i);

            ImTextureID tex = (ImTextureID)(intptr_t)assetManager.textures.id;
            if (ImGui::ImageButton(tex, {35, 35}, {atlas.x, atlas.y}, {atlas.x + atlas.width, atlas.y + atlas.height}))
            {
                gameData.creativeSelectedBlock = i;
                gameData.isHoldingTile = false;
            }

            ImGui::PopID();
            if (i % 10 != 0)
            {
                ImGui::SameLine();
            }
        }

        ImGui::Separator();
        ImGui::Separator();

        ImGui::Text("Tiles");

        ImGui::Separator();

        for (int i = 0; i < Tile::TILE_COUNT; i++)
        {
            Rectangle atlas = getTextureAtlas(i, 0, 32, 32);

            atlas.x /= assetManager.tiles.width;
            atlas.width /= assetManager.tiles.width;
            atlas.height /= assetManager.tiles.height;
            atlas.y /= assetManager.tiles.height;

            ImGui::PushID(i);

            ImTextureID tex = (ImTextureID)(intptr_t)assetManager.tiles.id;
            if (ImGui::ImageButton(tex, {35, 35}, {atlas.x, atlas.y}, {atlas.x + atlas.width, atlas.y + atlas.height}))
            {
                gameData.creativeSelectedTile = i;
                gameData.isHoldingTile = true;
            }

            ImGui::PopID();
            if (i % 10 != 0)
            {
                ImGui::SameLine();
            }
        }

        ImGui::End();
    }

    return true;
}

void closeGame()
{
}
