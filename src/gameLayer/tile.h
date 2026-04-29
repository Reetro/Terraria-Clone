#pragma once
#include <cstdint>

struct Tile
{
    enum
    {
        air,
        dirtWall,
        dirtWithGrassWall,
        stoneWall,
        grass,
        sandWall,
        sandWithCrystalWall,
        sandStoneWall,
        woodWall,
        darkStoneWall,
        clayWall,
        oakWall,
        leaveWall,
        cooperOreWall,
        ironOreWall,
        goldOreWall,
        cooperBlockWall,
        ironBlockWall,
        goldBlockWall,
        brickWall,
        snowWall,
        iceWall,
        rubyWall,
        platform,
        table,
        glassWall,
        furnace,
        painting,
        sapling,

        // This needs always be at the bottom of the enum it represents the number of tiles that are in the game
        TILE_COUNT,
    };

    std::uint8_t type = 0;
};
