#include "droppedItem.h"
#include "assetManager.h"
#include "entityIdHolder.h"
#include "helpers.h"

void DroppedItem::render(AssetManager &assetManager)
{
    auto aabb = getRectangleForEntity(physics.transform, 1, 1);

    DrawTexturePro(
        assetManager.textures,
        getTextureAtlas(itemType, 4, 32, 32),
        aabb, //dest
        {0, 0},// origin (top-left corner)
        0.0f, // rotation
        WHITE // tint
    );

    DrawRectangleLinesEx(aabb, 0.1,
        {20, 101, 250, 120});
}

bool DroppedItem::update(float deltaTime, EntityUpdateData entityUpdateData)
{
    for (auto &e : entityUpdateData.entityHolder.entities)
    {
        if (e.first != entityUpdateData.ownId)
        {
            if (e.second->getEntityType() == EntityType_DroppedItem)
            {
                if (auto *other = reinterpret_cast<DroppedItem *>(e.second.get()); itemType == other->itemType)
                {
                    if (Vector2Distance(getPosition(), other->getPosition()) < 0.7)
                    {
                        other->itemCounter += itemCounter;
                        return false;
                    }
                }
            }
        }
    }

    return true;
}
