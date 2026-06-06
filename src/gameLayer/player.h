#pragma once
#include "entity.h"

struct AssetManager;

struct Player : Entity
{
    Player()
    {
        physics.transform.w = 0.9f;
        physics.transform.h = 1.8f;
        life = Player::getMaxLife();
    }

    void render(AssetManager &assetManager) override;
    bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

    void onDeath() override
    {
    }

    void onHit(float damage) override
    {
    }

    int getEntityType() override
    {
        return EntityType_Player;
    }

    float getMaxLife() override
    {
        return 10.0f;
    }
};