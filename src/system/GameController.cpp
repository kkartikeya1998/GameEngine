#include "system/GameController.h"
#include "entities/movement/GridMovementMechanics.h"

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
    bool canMove = true;

    if (next.x < 0 || next.y < 0 ||
        next.x >= activeMap->getWidth() ||
        next.y >= activeMap->getHeight()) {
        canMove = false;
    }

    // optional: terrain blocking logic
    canMove &= activeMap->tile_at(next.x, next.y).isWalkable();

    if (canMove) {
        player_.movement().move(dir);
    }
}