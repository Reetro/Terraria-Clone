#pragma once
#include <cstdint>

struct Block
{
    enum
    {
        air = 0,
        dirt,

        // This needs always be at the bottom of the enum it represents the number of blocks that are in the game
        BLOCKS_COUNT,
    };

    std::uint8_t type = 0;
};