#pragma once

struct EntityAnimation
{
    int positionX = 0;
    int positionY = 0;

    float timer = 0.0f;

    void update(float deltaTimer, float frameDuration, int frameCount)
    {
        timer -= deltaTimer;

        if (timer <= 0)
        {
            timer += frameDuration;
            positionX++;
        }

        positionX %= frameCount;
    }

    void setAnimation(int animation)
    {
        if (positionY != animation)
        {
            positionX = 0;
            positionY = animation;
        }
    }
};