#pragma once
#include <physics.h>
#include <entity.h>

struct AssetManager;

struct DroppedItem : Entity
{
    DroppedItem()
    {
        physics.transform.w = 0.8f;
        physics.transform.h = 0.8f;
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
        return EntityType_DroppedItem;
    }

    float getMaxLife() override
    {
        return 1.0f;
    }

    int itemType = 0;
    int itemCounter = 1;
};