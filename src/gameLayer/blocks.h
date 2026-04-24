#pragma once
#include <cstdint>

struct Block
{
    enum
    {
        air = 0,
        dirt,
        grassBlock,
        stone,
        grass,
        sand,
        sandRuby,
        sandStone,
        woodPlank,
        stoneBricks,
        clay,
        woodLog,
        leaves,
        copper,
        iron,
        gold,
        copperBlock,
        ironBlock,
        goldBlock,
        bricks,
        snow,
        ice,
        rubyBlock,
        platform,
        workBench,
        glass,
        furnace,
        painting,
        sappling,
        snowBlueRuby,
        blueRubyBlock,
        door,
        jar,
        table,
        wordrobe,
        bookShelf,
        snowBricks,
        iceTable,
        iceWordrobe,
        iceBookShelf,
        icePlatform,
        sandTable,
        sandWordrobe,
        sandBookShelf,
        sandPlatform,
        woodenChest,
        iceChest,
        sandChest,
        boneChest,
        boneBricks,
        boneBench,
        boneWordrobe,
        boneBookShelf,
        bonePlatform,

        // This needs always be at the bottom of the enum it represents the number of blocks that are in the game
        BLOCKS_COUNT,
    };

    std::uint8_t type = 0;
};