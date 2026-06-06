#include <raylib.h>
#include "gameMain.h"
#include <assetManager.h>
#include <chrono>
#include <cmath>
#include <imgui.h>
#include <renderer.h>
#include <worldGenerator.h>
#include <randomStuff.h>
#include <entity.h>
#include "helpers.h"
#include "saveMap.h"
#include "entities/droppedItem.h"

AssetManager assetManager;

bool showImgui = false;

void spawnSlime(Vector2 position)
{
    Slime slime;

    slime.physics.teleport(position);

    auto id = gameData.entityHolder.idHolder.getEntityIdAndIncrement();

    gameData.entityHolder.entities[id] = std::make_unique<Slime>(slime);
}

void spawnDroppedItem(Vector2 position, int type)
{
    DroppedItem droppedItem;

    droppedItem.teleport(position);
    droppedItem.itemType = type;

    auto id = gameData.entityHolder.idHolder.getEntityIdAndIncrement();

    droppedItem.physics.velocity = {0, -3.0f};
    gameData.entityHolder.entities[id] = (std::make_unique<DroppedItem>(droppedItem));
}

bool initGame()
{
    assetManager.loadAll("../texturePacks");

    long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    generateWorld(gameData.gameMap, seed);

    gameData.camera.rotation = 0.0f;
    gameData.camera.zoom = 100.0f;

    gameData.player.teleport({55, 15});
    gameData.player.physics.transform.w = 0.9f;
    gameData.player.physics.transform.h = 1.8f;

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
        gameData.player.getPosition().x -= CAMERA_SPEED * GetFrameTime();
    }

    if (IsKeyDown(KEY_D))
    {
        gameData.player.getPosition().x += CAMERA_SPEED * GetFrameTime();
    }

    if (IsKeyDown(KEY_W))
    {
        gameData.player.getPosition().y -= CAMERA_SPEED * GetFrameTime();
    }

    if (IsKeyDown(KEY_S))
    {
        gameData.player.getPosition().y += CAMERA_SPEED * GetFrameTime();
    }

    if (IsKeyDown(KEY_SPACE))
    {
        gameData.player.physics.jump(10);
    }
#pragma endregion

#pragma region enities

    auto updateEntityPhysics = [&](auto &entity, bool applyGravity = true)
    {
        if (applyGravity) { entity.physics.applyGravity(); }

        entity.physics.updateForces(deltaTime);

        entity.physics.resolveConstrains(gameData.gameMap);

        entity.physics.updateFinal();
    };

    // Player
    updateEntityPhysics(gameData.player, false);

    gameData.camera.target = gameData.player.getPosition();

    // Update all entities
    std::ranlux24_base rng(std::random_device{}());

    //update all entities
    for (auto it = gameData.entityHolder.entities.begin(); it != gameData.entityHolder.entities.end();)
    {
        EntityUpdateData updateData
        {
            gameData.player.getPosition(),
            rng,
            gameData.entityHolder,
            it->first
        };

        bool shouldKill = false;

        if (!it->second->update(deltaTime, updateData) || it->second->life <= 0)
        {
            shouldKill = true;
        }

        if (shouldKill)
        {
            it->second->onDeath();
            // erase returns the next valid iterator
            it = gameData.entityHolder.entities.erase(it);
        }
        else
        {
            //physics
            updateEntityPhysics(*it->second, true);

            ++it;
        }
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
            bool hitAnEntity = false;

            for (auto &[id, entity] : gameData.entityHolder.entities)
            {
                Rectangle entityRect;
                entityRect.x = entity->getPosition().x;
                entityRect.y = entity->getPosition().y;
                entityRect.width  = entity->physics.transform.w;
                entityRect.height = entity->physics.transform.h;

                if (CheckCollisionPointRec(worldPos, entityRect) && entity->getEntityType() == EntityType_Slime)
                {
                    entity->onHit(1.0f);
                    hitAnEntity = true;
                    break;
                }
            }

            if (!hitAnEntity)
            {
                Block *b = gameData.gameMap.getBlockSafe(blockX, blockY);
                Tile *t = gameData.gameMap.getTileSafe(blockX, blockY);

                // 1. Check the foreground block first
                if (b && b->type != Block::air)
                {
                    spawnDroppedItem(
                        {static_cast<float>(blockX) + 0.5f, static_cast<float>(blockY) - 0.3f}, // slightly above center
                        b->type
                    );

                    *b = {}; // Destroy block
                }
                // 2. If no block is there, try to destroy the wall
                else if (t && t->type != 0) // Assuming 0 is empty for your Tile enum
                {
                    *t = {}; // Destroy wall
                }
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
            } else
            {
                if (Block *b = gameData.gameMap.getBlockSafe(blockX, blockY); b != nullptr)
                {
                    b->type = gameData.creativeSelectedBlock;
                }
            }
        }
    }

#pragma endregion

#pragma region block section

    if (showImgui)
    {
        if (IsKeyPressed(KEY_ONE))
        {
            gameData.selectionStart = Vector2{static_cast<float>(blockX), static_cast<float>(blockY)};
        }

        if (IsKeyPressed(KEY_TWO))
        {
            gameData.selectionEnd = Vector2{static_cast<float>(blockX), static_cast<float>(blockY)};
        }

        if (IsKeyPressed(KEY_THREE))
        {
            gameData.copyStructure.pasteIntoMap(gameData.gameMap, Vector2{
                                                    static_cast<float>(blockX), static_cast<float>(blockY)
                                                });
        }

        if (gameData.selectionStart.x > gameData.selectionEnd.x)
        {
            std::swap(gameData.selectionStart.x, gameData.selectionEnd.x);
        }

        if (gameData.selectionStart.y > gameData.selectionEnd.y)
        {
            std::swap(gameData.selectionStart.y, gameData.selectionEnd.y);
        }
    }

#pragma endregion

    BeginMode2D(gameData.camera);

    renderWorld(assetManager);

    if (showImgui)
    {
        Rectangle rect;
        rect.x = gameData.selectionStart.x;
        rect.y = gameData.selectionStart.y;
        rect.width = gameData.selectionEnd.x - gameData.selectionStart.x;
        rect.height = gameData.selectionEnd.y - gameData.selectionStart.y;

        rect.width++;
        rect.height++;

        DrawRectangleLinesEx(rect, 0.1, {20, 101, 250, 145});
    }

    EndMode2D();

    if (showImgui)
    {
        ImGui::Begin("Game Control");

        ImGui::SliderFloat("Camera Zoom:", &gameData.camera.zoom, 10.0f, 150);
        ImGui::SliderFloat("Camera Speed:", &CAMERA_SPEED, 5, 30);
        ImGui::Text("Player X: %f", gameData.player.physics.getPosition().x);
        ImGui::Text("Player Y: %f", gameData.player.physics.getPosition().y);
        ImGui::Text("FPS: %d", GetFPS());

        if (ImGui::Button("Spawn Slime"))
        {
            spawnSlime(gameData.player.getPosition());
        }

        if (ImGui::Button("Hurt a slime"))
        {
            for (auto &e: gameData.entityHolder.entities)
            {
                if (e.second->getEntityType() == EntityType_Slime)
                {
                    e.second->life -= 3;
                    break;
                }
            }
        }

        if (ImGui::Button("Copy"))
        {
            gameData.copyStructure.copyFromMap(gameData.gameMap, gameData.selectionStart, gameData.selectionEnd);
        }

        ImGui::InputText("File Name", gameData.saveName, sizeof(gameData.saveName));

        if (ImGui::Button("Save To File"))
        {
            std::string path = RESOURCES_PATH "structures/";
            path += gameData.saveName;
            path += ".bin";

            saveBlockDataToFile(gameData.copyStructure.mapData, gameData.copyStructure.w, gameData.copyStructure.h,
                                path.c_str());
        }

        if (ImGui::Button("Load From File"))
        {
            std::string path = RESOURCES_PATH "structures/";
            path += gameData.saveName;
            path += ".bin";

            loadBlockDataFromFile(gameData.copyStructure.mapData, gameData.copyStructure.w, gameData.copyStructure.h,
                                  path.c_str());
        }

        ImGui::Separator();

        ImGui::Text("Blocks");

        ImGui::Separator();

        // Creative menu
        for (int i = 0; i < Block::BLOCKS_COUNT; i++)
        {
            if (i == Block::air)
            {
                continue;;
            }

            Rectangle atlas = getTextureAtlas(i, 0, 32, 32);

            atlas.x /= assetManager.textures.width;
            atlas.width /= assetManager.textures.width;
            atlas.height /= assetManager.textures.height;
            atlas.y /= assetManager.textures.height;

            ImGui::PushID(i);

            ImTextureID tex = (ImTextureID) (intptr_t) assetManager.textures.id;
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
            if (i == Tile::air)
            {
                continue;
            }

            Rectangle atlas = getTextureAtlas(i, 0, 32, 32);

            atlas.x /= assetManager.tiles.width;
            atlas.width /= assetManager.tiles.width;
            atlas.height /= assetManager.tiles.height;
            atlas.y /= assetManager.tiles.height;

            ImGui::PushID(i);

            ImTextureID tex = (ImTextureID) (intptr_t) assetManager.tiles.id;
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
