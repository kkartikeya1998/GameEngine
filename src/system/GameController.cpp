#include <iostream>

#include "system/GameController.h"
#include "system/GameConstants.h"
#include "system/MovementSystem.h"
#include "system/AnimationSystem.h"
#include "component/PositionComponent.h"
#include "component/FreeMovementComponent.h"
#include "component/CollisionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/WalkCycleTimer.h"

GameController::GameController(int startMapId, int playerX, int playerY, const AssetDatabase &assets)
    : world_(assets), player_(makePlayer(static_cast<float>(playerX), static_cast<float>(playerY)))
{
    world_.loadMap(startMapId);
}

bool GameController::isPositionBlocked(const AABB &box) const
{
    const Map *activeMap = world_.getActiveMap();
    if (!activeMap)
        return true;

    return activeMap->isAreaBlocked(box);
}

void GameController::update(float dt, const PlayerControlComponent &input)
{
    MovementSystem::update(player_, input, *world_.getActiveMap(), dt,
                           [this](const AABB &box)
                           { return isPositionBlocked(box); });

    auto *walkCycle = player_.get<WalkCycleTimer>();
    auto *movementState = player_.get<MovementStateComponent>();
    if (walkCycle)
    {
        bool isMoving = movementState->current != MovementState::Idle;
        float speedScale = (movementState->current == MovementState::Sprinting) ? 1.5f : 1.f;
        walkCycle->update(dt, isMoving, speedScale);
    }
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