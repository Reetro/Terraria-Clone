#include "slime.h"
#include "assetManager.h"
#include "blocks.h"
#include "gameMain.h"
#include "helpers.h"
#include "randomStuff.h"
#include "entities/droppedItem.h"

void Slime::render(AssetManager &asset_manager)
{
    auto aabb = getRectangleForEntity(physics.transform, 1, 1);

    if (redTime > 0)
    {
        DrawTexturePro(asset_manager.slime,
        getTextureAtlas(animation.positionX, animation.positionY, 32, 32),
        aabb,
        {0, 0},
        0.0f,
        RED);
    }
    else
    {
        DrawTexturePro(asset_manager.slime,
        getTextureAtlas(animation.positionX, animation.positionY, 32, 32),
        aabb,
        {0, 0},
        0.0f,
        WHITE);
    }
}

bool Slime::update(float deltaTime, EntityUpdateData entityUpdateData)
{
    changeStateTimer -= deltaTime;

    if (changeStateTimer <= 0)
    {
        changeStateTimer = getRandomFloat(entityUpdateData.rng, 1, 7);

        float distanceToPlayer = Vector2Distance(entityUpdateData.playerPosition, getPosition());

        if (distanceToPlayer < 20)
        {
            if (getRandomChance(entityUpdateData.rng, 0.8f))
            {
                current_state = STATE_CHASING;
            }
            else
            {
                current_state = STATE_WONDERING;
            }
        }
        else
        {
            current_state = STATE_WONDERING;
        }
    }

    if (physics.downTouch)
    {
        moveSpeed = 0;

        animation.setAnimation(0); // staying
    }
    else
    {
        animation.setAnimation(1); // in air
    }

    jumpTimer -= deltaTime;

    switch (current_state)
    {
        default:
        case STATE_WONDERING:
        {
            if (jumpTimer <= 0)
            {
                jumpTimer = getRandomFloat(entityUpdateData.rng, 3, 12);

                physics.jump(10);
                moveSpeed = getRandomFloat(entityUpdateData.rng, -7, 7);
            }
        }
        break;

        case STATE_CHASING:
        {
            if (jumpTimer <= 0)
            {
                jumpTimer = getRandomFloat(entityUpdateData.rng, 2, 7);

                physics.jump(10);

                if (entityUpdateData.playerPosition.x > getPosition().x)
                {
                    moveSpeed = getRandomFloat(entityUpdateData.rng, 1, 7);
                }
                else
                {
                    moveSpeed = -getRandomFloat(entityUpdateData.rng, 1, 7);
                }
            }
        }
        break;
    }

    if (moveSpeed)
    {
        getPosition().x += deltaTime * moveSpeed;
    }

    animation.update(deltaTime, 0.08, 7);

    redTime -= deltaTime;

    return true;
}

void Slime::onDeath()
{
    DroppedItem droppedItem;

    droppedItem.teleport(getPosition());
    droppedItem.itemType = Block::dirt;

    auto id = gameData.entityHolder.idHolder.getEntityIdAndIncrement();

    droppedItem.physics.velocity = {0, -3.0f};
    gameData.entityHolder.entities[id] = (std::make_unique<DroppedItem>(droppedItem));
}

void Slime::onHit(float damage)
{
    life -= damage;
    redTime = 0.5f;
}
