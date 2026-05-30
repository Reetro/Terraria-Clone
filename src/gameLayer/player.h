#pragma once
#include "entity.h"

struct AssetManager;

struct Player : Entity
{
    Player()
    {
        physics.transform.w = 0.9f;
        physics.transform.h = 1.8f;
    }

    void render(AssetManager &assetManager) override;
    bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

    int getEntityType() override
    {
        return EntityType_Player;
    }
};