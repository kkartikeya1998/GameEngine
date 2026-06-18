#include "system/GameController.h"

GameController::GameController(int startMapId, int playerX, int playerY)
    : world_(std::string(PROJECT_ROOT) + "/assets/maps/"),
      player_(playerX, playerY)
{
    world_.load_map(startMapId);
}

void GameController::changeMap(int mapId, int newX, int newY) {
    // Load the new map (unloads current, loads new)
    world_.load_map(mapId);
    
    // Reposition the player on the new map
    player_.moveTo(newX, newY);
}