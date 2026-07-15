#include <iostream>

#include "system/GameController.h"
#include "system/GameConstants.h"
#include "system/MovementSystem.h"
#include "system/CollisionSystem.h"
#include "system/AnimationSystem.h"
#include "component/PositionComponent.h"
#include "component/FreeMovementComponent.h"
#include "component/CollisionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/WalkCycleTimer.h"

GameController::GameController(int startMapId, int playerX, int playerY, const AssetDatabase &assets)
    : world_(assets)
{
    world_.loadMap(startMapId);
    playerId_ = makePlayer(world_.registry(), static_cast<float>(playerX), static_cast<float>(playerY));
}

bool GameController::isPositionBlocked(const AABB &box)
{
    return CollisionSystem::isAreaBlocked(world_.registry(), box, playerId_);
}

void GameController::update(float dt, const PlayerControlComponent &input)
{
    MovementSystem::update(world_.registry(), playerId_, input, world_.getActiveMap(), dt,
                           [this](const AABB &box)
                           { return isPositionBlocked(box); });

    auto *walkCycle = world_.registry().get<WalkCycleTimer>(playerId_);
    auto *movementState = world_.registry().get<MovementStateComponent>(playerId_);
    if (walkCycle && movementState)
    {
        bool isMoving = movementState->current != MovementState::Idle;
        float speedScale = (movementState->current == MovementState::Sprinting) ? 1.5f : 1.f;
        walkCycle->update(dt, isMoving, speedScale);
    }
}

void GameController::changeMap(int mapId, float newX, float newY)
{
    world_.loadMap(mapId);

    // Map reload destroyed the old player entity along with everything
    // else — recreate it at the new position rather than reusing playerId_.
    playerId_ = makePlayer(world_.registry(), newX, newY);
}