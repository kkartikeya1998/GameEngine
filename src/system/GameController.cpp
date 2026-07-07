#include <iostream>

#include "system/GameController.h"
#include "system/GameConstants.h"
#include "system/MovementSystem.h"
#include "system/AnimationSystem.h"
#include "tmp/component/PositionComponent.h"
#include "tmp/component/FreeMovementComponent.h"
#include "tmp/component/CollisionComponent.h"
#include "tmp/component/MovementStateComponent.h"
#include "tmp/component/WalkCycleTimer.h"

GameController::GameController(int startMapId, int playerX, int playerY,
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

void GameController::update(float dt, const PlayerControlComponent &input)
{
    auto *position = player_.get<PositionComponent>();
    auto *velocity = player_.get<VelocityComponent>();
    auto *direction = player_.get<DirectionComponent>();
    auto *movement = player_.get<FreeMovementComponent>();
    auto *collision = player_.get<CollisionComponent>();
    auto *movementState = player_.get<MovementStateComponent>();
    auto *walkCycle = player_.get<WalkCycleTimer>();
    if (!position || !velocity || !direction || !movement || !collision || !movementState)
        return;

    MovementSystem::update(*position, *velocity, *direction, *movement, *collision,
                               *movementState, dt, input.direction, input.sprinting,
                               [this](const AABB &box)
                               { return isPositionBlocked(box); });


    if (walkCycle) {
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