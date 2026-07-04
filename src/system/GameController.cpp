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
        return true;

    return activeMap->isAreaBlocked(box);
}

void GameController::updatePlayerMovement(float dt, const PlayerControlComponent &input)
{
    auto *position = player_.get<PositionComponent>();
    auto *velocity = player_.get<VelocityComponent>();
    auto *direction = player_.get<DirectionComponent>();
    auto *movement = player_.get<FreeMovementComponent>();
    auto *walkCycle = player_.get<WalkCycleTimer>();
    if (!position || !velocity || !direction || !movement)
        return;

    MovementSystem::updateFree(*position, *velocity, *direction, *movement, dt, input.direction,
                               [this](const AABB &box)
                               { return isPositionBlocked(box); });

    // Was called twice in the old version (once null-guarded, once
    // not) — collapsed to a single guarded call, same end effect.
    bool isMoving = (input.direction != Direction::NONE);
    if (walkCycle)
        walkCycle->update(dt, isMoving);

    // input.sprinting / input.jumpRequested aren't consumed yet —
    // this is the seam for PlayerStateSystem/PlayerStateComponent.
}

void GameController::update(float dt)
{
    Map *activeMap = world_.getActiveMap();
    if (!activeMap)
        return;
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