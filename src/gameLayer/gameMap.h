#pragma once
#include <vector>
#include <blocks.h>
#include <tile.h>

struct GameMap
{
    /// Width of the map
    int w = 0;
    /// Height of the map
    int h = 0;

    /// Contains all map data
    std::vector<Block> mapData;

    /// Contains all tile data
    std::vector<Tile> tileData;

    /**
     * @brief Creates a new map with the given width and height
     * @param w Width of the map
     * @param h Height of the map
    */
    void create(int w, int h);

    /**
     * @brief Gets the block at the given x y cords does not check to see if the block is in map bounds
     * @param x Target x cord
     * @param y Target y cord
     * @return a reference to the target block
     */
    Block &getBlocUnsafe(int x, int y);

    /**
     * @brief Gets the block at the given x y cords checks to see if the block is in map bounds
     * @param x Target x cord
     * @param y Target y cord
     * @return a pointer to the target block
     */
    Block *getBlockSafe(int x, int y);

    /**
     * @brief Gets the tile at the given x y cords does not check to see if the tile is in map bounds
     * @param x Target x cord
     * @param y Target y cord
     * @return a reference to the target block
     */
    Tile &getTileUnsafe(int x, int y);

    /**
     * @brief Gets the tile at the given x y cords checks to see if the tile is in map bounds
     * @param x Target x cord
     * @param y Target y cord
     * @return a pointer to the target block
     */
    Tile *getTileSafe(int x, int y);
};