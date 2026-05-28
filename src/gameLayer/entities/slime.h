#pragma once
#include <physics.h>
#include <raylib.h>
#include <random>
#include <entityAnimation.h>

struct AssetManager;

struct Slime
{
    Slime()
    {
        physics.transform.w = 0.8f;
        physics.transform.h = 0.8f;
    }

    PhysicalEntity physics;
    EntityAnimation animation;

    Vector2 &getPosition()
    {
        return physics.transform.pos;
    }

    void render(const AssetManager &asset_manager) const;

    void update(float deltaTime, std::ranlux24_base rng, Vector2 playerPosition);

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