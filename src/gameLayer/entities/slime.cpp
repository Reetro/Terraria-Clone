#include "slime.h"

#include "assetManager.h"
#include "helpers.h"

void Slime::render(const AssetManager &asset_manager)
{
    auto aabb = getRectangleForEntity(physics.transform, 1, 1);

    DrawTexturePro(asset_manager.slime, getTextureAtlas(0, 0, 32, 32), aabb, {0, 0}, 0.0f, WHITE);
}

void Slime::update(float deltaTime)
{
}
