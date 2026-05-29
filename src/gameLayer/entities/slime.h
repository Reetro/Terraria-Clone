#pragma once
#include <physics.h>
#include <entityAnimation.h>
#include <entity.h>

struct AssetManager;

struct Slime : Entity
{
    Slime()
    {
        physics.transform.w = 0.8f;
        physics.transform.h = 0.8f;
    }

    EntityAnimation animation;

    void render(AssetManager &asset_manager) override;
    void update(float deltaTime, EntityUpdateData entityUpdateData) override;

    int getEntityType() override
    {
        return EntityType_Slime;
    }

    enum
    {
        STATE_WONDERING = 0,
        STATE_CHASING
    };

    int current_state = STATE_WONDERING;
    float changeStateTimer = 1.0f;
    float jumpTimer = 5.0f;
    float moveSpeed = 0.0f;
};