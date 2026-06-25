#include "system/GameController.h"
#include "entities/movement/FreeMovementMechanics.h"
#include "system/GameConstants.h"
#include <iostream>

namespace {
    constexpr float PLAYER_HITBOX_WIDTH = 32.f;
    constexpr float PLAYER_HITBOX_HEIGHT = 24.f;

    float gridToPixelX(int gridX) {
        return (static_cast<float>(gridX) + 0.5f) * GameConstants::TILE_SIZE;
    }
    float gridToPixelY(int gridY) {
        return (static_cast<float>(gridY) + 1.0f) * GameConstants::TILE_SIZE;
    }
}

GameController::GameController(int startMapId, int playerX, int playerY,
                                const std::string& assetsRoot,
                                MapObjectRepository& objectRepository)
    : world_(assetsRoot + "/maps/", objectRepository),
    player_(std::make_unique<FreeMovementMechanics>(
        gridToPixelX(playerX),
        gridToPixelY(playerY),
        GameConstants::PLAYER_SPEED,
        PLAYER_HITBOX_WIDTH,
        PLAYER_HITBOX_HEIGHT
    ))
{
    world_.loadMap(startMapId);
}

// Also update changeMap (if it currently sets raw newX/newY) to use the
// same gridToPixelX/gridToPixelY helpers:
//
// void GameController::changeMap(int mapId, int newX, int newY) {
//     world_.loadMap(mapId);
//     player_.movement().setPosition(gridToPixelX(newX), gridToPixelY(newY));
// }

void GameController::changeMap(int mapId, int newX, int newY) {
    world_.loadMap(mapId);

    // newX/newY arrive as grid coordinates (same convention as before —
    // teleport targets in object_metadata.json are tile coordinates).
    // Player's position is now in pixel/world space, so convert.
    player_.movement().setPosition(
        static_cast<float>(newX) * GameConstants::TILE_SIZE,
        static_cast<float>(newY) * GameConstants::TILE_SIZE
    );
}

bool GameController::isPositionBlocked(const AABB& box) const {
    Map* activeMap = world_.getActiveMap();
    if (!activeMap) return true;
    bool rev = activeMap->isAreaBlocked(box, GameConstants::TILE_SIZE);
    std::cout << "Is next position blocked? " << rev << "\n";
    return rev;
}

void GameController::updatePlayerMovement(float dt, Direction inputDir) {
    Map* activeMap = world_.getActiveMap();
    if (!activeMap) return;

    player_.updateMovement(dt, inputDir,
        [this](const AABB& box) { return isPositionBlocked(box); });

}