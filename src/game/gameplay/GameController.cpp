#include "game/gameplay/GameController.h"
#include "game/GameConstants.h"
#include "game/ecs/movement/MovementSystem.h"
#include "game/ecs/collision/CollisionSystem.h"
#include "game/ecs/animation/AnimationSystem.h"
#include "game/ecs/movement/DirectionSystem.h"
#include "game/ecs/interaction/InteractionSystem.h"
#include "game/ecs/interaction/ItemPickupSystem.h"
#include "game/ecs/animation/MovementAnimationSystem.h"
#include "game/ecs/tbd/LifetimeSystem.h"
#include "game/ecs/animation/FloatingTextSystem.h"
#include "game/ecs/npc/CreatureAISystem.h"
#include "game/ecs/npc/WanderAIComponent.h"
#include "game/ecs/interaction/WorldItemComponent.h"
#include "game/ecs/movement/PositionComponent.h"
#include "game/ecs/movement/FreeMovementComponent.h"
#include "game/ecs/collision/CollisionComponent.h"
#include "game/ecs/movement/MovementStateComponent.h"

GameController::GameController(int startMapId, int playerX, int playerY, const AssetDatabase &assets, EventQueue<GameEvent> &events)
    : world_(assets), assets_(assets), events_(events)
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
    MovementSystem::update(world_.registry(), playerId_, world_.getActiveMap(), dt,
                           [this](const AABB &box) { return isPositionBlockedFor(playerId_, box); }, &input);

    checkItemPickups();
    InteractionSystem::Update(world_.registry(), playerId_, events_, input);

    CreatureAISystem::update(world_.registry(), dt);

    // Runs once, after both player and AI velocities are finalized for
    // this frame, so it resolves full 8-way facing for everyone —
    // including diagonal wander movement — instead of running too early
    // and being overwritten (or missing AI velocity entirely).
    DirectionSystem::update(world_.registry());

    for (EntityID id : world_.registry().view<WanderAIComponent>())
    {
        MovementSystem::update(world_.registry(), id, world_.getActiveMap(), dt,
                               [this, id](const AABB &box) { return isPositionBlockedFor(id, box); });
    }

    MovementAnimationSystem::update(world_.registry());
    FloatingTextSystem::update(world_.registry());
    LifetimeSystem::update(world_.registry(), dt);
}

void GameController::changeMap(int mapId, float newX, float newY)
{
    world_.loadMap(mapId);
    playerId_ = makePlayer(world_.registry(), assets_, newX, newY);
}

void GameController::checkItemPickups()
{
    auto *playerPos = world_.registry().get<PositionComponent>(playerId_);
    if (!playerPos)
        return;
    auto *playerCol = world_.registry().get<CollisionComponent>(playerId_);
    AABB playerBox = playerCol ? playerCol->resolve(playerPos->x, playerPos->y)
                               : AABB{playerPos->x, playerPos->y, 0.f, 0.f};

    for (EntityID id : world_.registry().view<WorldItemComponent, PositionComponent>())
    {
        auto *itemPos = world_.registry().get<PositionComponent>(id);
        auto *itemCol = world_.registry().get<CollisionComponent>(id);
        AABB itemBox = itemCol ? itemCol->resolve(itemPos->x, itemPos->y)
                               : AABB{itemPos->x, itemPos->y, GameConstants::TILE_SIZE, GameConstants::TILE_SIZE};

        if (playerBox.intersects(itemBox))
            ItemPickupSystem::pickup(world_.registry(), assets_, events_, playerId_, id);
    }
}