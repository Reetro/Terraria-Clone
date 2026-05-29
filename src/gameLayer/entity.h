#pragma once
#include <random>
#include "physics.h"

struct AssetManager;

enum EntityType
{
    EntityType_Player = 0,
    EntityType_Slime,
};

struct EntityUpdateData
{
    Vector2 playerPosition;
    std::ranlux24_base &rng;
};

struct Entity
{
    virtual ~Entity() = default;

    PhysicalEntity physics;

    Vector2 &getPosition()
    {
        return physics.transform.pos;
    }

    void teleport(Vector2 position)
    {
        physics.transform.pos = position;
    }

    virtual void render(AssetManager &asset_manager) = 0;
    virtual void update(float deltaTime, EntityUpdateData entityUpdateData) = 0;
    virtual int getEntityType() = 0;
};