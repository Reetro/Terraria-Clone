#pragma once
#include <blocks.h>
#include <gameMap.h>

inline struct GameData
{
    GameMap gameMap;
    Camera2D camera{};
    Block blockToPlace;
} gameData;

// Initializes game login called when game starts
bool initGame();

// Called every frame
bool updateGame();

// Called when the game closes
void closeGame();