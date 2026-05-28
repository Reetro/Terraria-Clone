#include "helpers.h"

Rectangle getTextureAtlas(int x, int y, int cellSizePixelsX, int cellSizePixelsY)
{
    return Rectangle{static_cast<float>(x) * cellSizePixelsX, static_cast<float>(y) * cellSizePixelsY, static_cast<float>(cellSizePixelsX), static_cast<float>(cellSizePixelsY)};
}

Rectangle getTreeAtlas(const int col, const int row)
{
    return { static_cast<float>(col * 32), static_cast<float>(row * 32), 32, 32 };
}

Rectangle getRectangleForEntity(Transform2D transform, float textureW, float textureH)
{
    Transform2D result = transform;
    result.w = textureW;
    result.h = textureH;

    result.pos.y -= (result.h - transform.h) / 2;

    return result.getAABB();
}