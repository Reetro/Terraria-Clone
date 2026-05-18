#pragma once
#include <vector>
#include <blocks.h>
#include <tile.h>
#include <raylib.h>
#include "gameMap.h"

struct Structure
{
    /// Width of the structure
    int w = 0;
    /// Height of the structure
    int h = 0;

    /// Contains all structure data
    std::vector<Block> mapData;

    /// Contains all tile data
    std::vector<Tile> tileData;

    /**
     * @brief Creates a new structure with the given width and height
     * @param w Width of the structure
     * @param h Height of the structure
    */
    void create(int w, int h);

    /**
     * @brief Gets the block at the given x y cords does not check to see if the block is in structure bounds
     * @param x Target x cord
     * @param y Target y cord
     * @return a reference to the target block
     */
    Block &getBlocUnsafe(int x, int y);

    /**
     * @brief Gets the block at the given x y cords checks to see if the block is in structure bounds
     * @param x Target x cord
     * @param y Target y cord
     * @return a pointer to the target block
     */
    Block *getBlockSafe(int x, int y);

    /**
     * @brief Gets the tile at the given x y cords does not check to see if the tile is in structure bounds
     * @param x Target x cord
     * @param y Target y cord
     * @return a reference to the target block
     */
    Tile &getTileUnsafe(int x, int y);

    /**
     * @brief Gets the tile at the given x y cords checks to see if the tile is in structure bounds
     * @param x Target x cord
     * @param y Target y cord
     * @return a pointer to the target block
     */
    Tile *getTileSafe(int x, int y);

    /**
     * @brief Copies the blocks in the start and end pos
     * @param map Current map
     * @param start Structure start point
     * @param end Structure end point
     */
    void copyFromMap(GameMap &map, Vector2 start, Vector2 end);

    /**
     * @brief pasts the blocks in the start and end pos
     * @param map Current map
     * @param start Structure starting point
     */
    void pasteIntoMap(GameMap &map, Vector2 start);
};
