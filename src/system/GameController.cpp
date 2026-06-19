#include "system/GameController.h"
#include "entities/movement/GridMovementMechanics.h"
#include <iostream>

GameController::GameController(int startMapId, int playerX, int playerY)
    : world_(std::string(PROJECT_ROOT) + "/assets/maps/"),
    player_(std::make_unique<GridMovementMechanics>(playerX, playerY))
{
    world_.loadMap(startMapId);
}

void GameController::changeMap(int mapId, int newX, int newY) {
    world_.loadMap(mapId);

    // movement system handles position
    player_.movement().setPosition(newX, newY);
}


void GameController::movePlayer(Direction dir) {
    Map* activeMap = world_.getActiveMap();
    if (!activeMap) return;

    // ask movement system for next position
    Position current = player_.movement().getPosition();
    Position next = player_.movement().nextPos(dir);

    // world validation (collision check placeholder)
    bool inBounds = (next.x >= 0 && next.y >= 0 &&
        next.x < activeMap->getWidth() &&
        next.y < activeMap->getHeight());

    if (!inBounds) {
        std::cout << "Player cannot move to (" << next.x << ", " << next.y << ")\n";
        return;
    }
    
    // optional: terrain blocking logic
    bool canMove = activeMap->tile_at(next.x, next.y).isWalkable();

    if (canMove) {
        std::cout << "Player moved to (" << next.x << ", " << next.y << ")\n";
        player_.movement().move(dir);
    } else {
        std::cout << "Player cannot move to (" << next.x << ", " << next.y << ") - blocked by terrain/object\n";
    }
}