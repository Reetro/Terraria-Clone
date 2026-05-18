#pragma once
#include <blocks.h>
#include <tile.h>
#include <gameMap.h>

#include "structure.h"

inline struct GameData
{
    GameMap gameMap;
    Camera2D camera{};
    Tile tileToPlace;
    bool isHoldingTile = false;
    int creativeSelectedBlock = Block::dirt;
    int creativeSelectedTile = Tile::dirtWall;

    Vector2 selectionStart = {};
    Vector2 selectionEnd = {};

    Structure copyStructure;

    char saveName[100] = {};
} gameData;

// Initializes game logic called when game starts
bool initGame();

// Called every frame
bool updateGame();

// Called when the game closes
void closeGame();