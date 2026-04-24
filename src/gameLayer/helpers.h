#pragma once
#include <raylib.h>

/**
* @brief Creates a rectangle that contains the location of the corresponding texture in an atlas
* @param x Location of the texture on the x-axis
* @param y Location of the texture on the y-axis
* @param cellSizePixelsX Size of the texture on the x-axis
* @param cellSizePixelsY Size of the texture on the y-axis
*/
Rectangle getTextureAtlas(int x, int y, int cellSizePixelsX, int cellSizePixelsY);