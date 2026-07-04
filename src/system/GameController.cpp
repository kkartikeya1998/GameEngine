#include <iostream>

#include "system/GameController.h"
#include "system/GameConstants.h"
#include "system/MovementSystem.h"
#include "tmp/movement/PositionComponent.h"
#include "tmp/movement/FreeMovementComponent.h"
#include "tmp/movement/WalkCycleTimer.h"

GameController::GameController(int startMapId, int playerX, int playerY,
                               const std::string &assetsRoot,
                               MapObjectRepository &objectRepository,
                               TileRepository &tileRepository)
    : world_(objectRepository, tileRepository), player_(makePlayer(static_cast<float>(playerX), static_cast<float>(playerY)))
{
    world_.loadMap(startMapId);
}

bool GameController::isPositionBlocked(const AABB &box) const
{
    Map *activeMap = world_.getActiveMap();
    if (!activeMap)
        return true; // no map loaded -> nowhere is walkable

    return activeMap->isAreaBlocked(box);
}

void GameController::updatePlayerMovement(float dt, Direction inputDir)
{
    auto *position = player_.get<PositionComponent>();
    auto *velocity = player_.get<VelocityComponent>();
    auto *direction = player_.get<DirectionComponent>();
    auto *movement = player_.get<FreeMovementComponent>();
    auto *walkCycle = player_.get<WalkCycleTimer>();
    if (!position || !velocity || !direction || !movement)
        return;

    if(movement && walkCycle){
        walkCycle->update(dt, movement->isMoving);
    }
    MovementSystem::updateFree(*position, *velocity, *direction, *movement, dt, inputDir,
                               [this](const AABB &box)
                               { return isPositionBlocked(box); });// GameController::updatePlayerMovement(dt, inputDir), after calling updateFree:
    bool isMoving = (inputDir != Direction::NONE);
    player_.get<WalkCycleTimer>()->update(dt, isMoving);
}

void GameController::update(float dt)
{
    Map *activeMap = world_.getActiveMap();
    if (!activeMap)
        return;

    // NPC and other animates update
    // auto isBlocked = [this](const AABB& box) { return isPositionBlocked(box); };

    // for (auto& npcEntity : activeMap->getNpcs()) {
    //     auto* position = npcEntity->get<PositionComponent>();
    //     if (!position) continue;

    //     if (auto* grid = npcEntity->get<GridMovementComponent>()) {
    //         MovementSystem::updateGrid(*position, *grid, Direction::NONE, isBlocked);
    //     } else if (auto* free = npcEntity->get<FreeMovementComponent>()) {
    //         MovementSystem::updateFree(*position, *free, dt, Direction::NONE, isBlocked);
    //     }
    // }
}

void GameController::changeMap(int mapId, float newX, float newY)
{
    world_.loadMap(mapId);

    auto *position = player_.get<PositionComponent>();
    if (!position)
        return;

    position->x = newX;
    position->y = newY;
}