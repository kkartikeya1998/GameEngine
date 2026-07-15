#pragma once

#include "entities/Registry.h"
#include "component/PositionComponent.h"
#include "component/VelocityComponent.h"
#include "component/DirectionComponent.h"
#include "component/FreeMovementComponent.h"
#include "component/CollisionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/WalkCycleTimer.h"
#include "component/RenderComponent.h"
#include "component/PlayerControlComponent.h"
#include "system/GameConstants.h"
#include "asset/AsssetPaths.h"

inline EntityID makePlayer(
    Registry& registry,
    float x, float y,
    float movement_speed = GameConstants::PLAYER_SPEED,
    float sprintMultiplier = 1.5f,
    float walkCyclesPerSecond = 4.0f)
{
    EntityID id = registry.create();
    registry.add<PositionComponent>(id, x, y);
    registry.add<VelocityComponent>(id);
    registry.add<DirectionComponent>(id, Direction::DOWN);
    registry.add<FreeMovementComponent>(id, movement_speed, sprintMultiplier);
    registry.add<CollisionComponent>(id,
        GameConstants::PLAYER_HITBOX_OFFSET_X,
        GameConstants::PLAYER_HITBOX_OFFSET_Y,
        GameConstants::PLAYER_HITBOX_WIDTH,
        GameConstants::PLAYER_HITBOX_HEIGHT);
    registry.add<MovementStateComponent>(id);
    registry.add<WalkCycleTimer>(id, walkCyclesPerSecond);
    registry.add<RenderComponent>(id,
        "player_walk_down_0",
        Assets::Characters::PLAYER_SPRITESHEET.string(),
        sf::IntRect{{0, 0}, {32, 32}},
        GameConstants::TILE_SIZE,
        x, y,
        RenderLayer::Characters);

    // registry.add<PlayerControlComponent>(id);
    return id;
}