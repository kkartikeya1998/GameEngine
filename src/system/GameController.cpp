#include <iostream>

#include "system/GameController.h"
#include "system/GameConstants.h"
#include "system/MovementSystem.h"
#include "system/CollisionSystem.h"
#include "system/AnimationSystem.h"
#include "entities/npc/CreatureAISystem.h"
#include "entities/npc/WanderAIComponent.h"
#include "component/PositionComponent.h"
#include "component/FreeMovementComponent.h"
#include "component/CollisionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/WalkCycleTimer.h"

GameController::GameController(int startMapId, int playerX, int playerY, const AssetDatabase &assets)
    : world_(assets), assets_(assets)
{
    world_.loadMap(startMapId);
    playerId_ = makePlayer(world_.registry(), assets_, static_cast<float>(playerX), static_cast<float>(playerY));
}

bool GameController::isPositionBlockedFor(EntityID id, const AABB &box)
{
    return CollisionSystem::isAreaBlocked(world_.registry(), box, id);
}

void GameController::update(float dt, const PlayerControlComponent &input)
{
    // Player moves first — NPC collision checks below read the player's
    // post-move position, giving the player effective movement priority.
    MovementSystem::update(world_.registry(), playerId_, world_.getActiveMap(), dt,
                            [this](const AABB &box) { return isPositionBlockedFor(playerId_, box); },
                            &input);

    auto *walkCycle = world_.registry().get<WalkCycleTimer>(playerId_);
    auto *movementState = world_.registry().get<MovementStateComponent>(playerId_);
    if (walkCycle && movementState)
    {
        bool isMoving = movementState->current != MovementState::Idle;
        float speedScale = (movementState->current == MovementState::Sprinting) ? 1.5f : 1.f;
        walkCycle->update(dt, isMoving, speedScale);
    }

    CreatureAISystem::update(world_.registry(), dt);
    for (EntityID id : world_.registry().view<WanderAIComponent>())
    {
        MovementSystem::update(world_.registry(), id, world_.getActiveMap(), dt,
                                [this, id](const AABB &box) { return isPositionBlockedFor(id, box); });

        if (auto *timer = world_.registry().get<WalkCycleTimer>(id))
            timer->update(dt, true);
    }
}

void GameController::changeMap(int mapId, float newX, float newY)
{
    world_.loadMap(mapId);
    playerId_ = makePlayer(world_.registry(), assets_, newX, newY);
}