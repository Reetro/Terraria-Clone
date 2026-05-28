#pragma once
#include "raylib.h"

struct AssetManager
{
    Texture2D dirt = {};
    Texture2D textures = {};
    Texture2D frame = {};
    Texture2D tree = {};
    Texture2D tiles = {};
    Texture2D player = {};
    Texture slime = {};

    void loadAll();
};