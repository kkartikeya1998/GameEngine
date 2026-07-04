#pragma once

#include "entities/Entity.h"
#include "tmp/movement/PositionComponent.h"
#include "tmp/movement/VelocityComponent.h"
#include "tmp/movement/DirectionComponent.h"
#include "tmp/movement/FreeMovementComponent.h"
#include "tmp/movement/CollisionComponent.h"
#include "tmp/movement/MovementStateComponent.h"
#include "tmp/movement/WalkCycleTimer.h"
#include "tmp/movement/RenderComponent.h"
#include "tmp/movement/PlayerControlComponent.h"
#include "system/GameConstants.h"

inline Entity makePlayer(
    float x, float y,
    float movement_speed = GameConstants::PLAYER_SPEED,
    float sprintMultiplier = 1.5f,
    float walkCyclesPerSecond = 4.0f)
{
    Entity e;
    e.add<PositionComponent>(x, y);
    e.add<VelocityComponent>();
    e.add<DirectionComponent>(Direction::DOWN);
    e.add<FreeMovementComponent>(movement_speed, sprintMultiplier);
    e.add<CollisionComponent>(
        GameConstants::PLAYER_HITBOX_OFFSET_X,
        GameConstants::PLAYER_HITBOX_OFFSET_Y,
        GameConstants::PLAYER_HITBOX_WIDTH,
        GameConstants::PLAYER_HITBOX_HEIGHT);
    e.add<MovementStateComponent>();
    e.add<WalkCycleTimer>(walkCyclesPerSecond);
    e.add<RenderComponent>(
        "player_walk_down_0", // initial frame name
        Assets::Sprites::PLAYER_SPRITESHEET.string(),
        sf::IntRect{{0, 0}, {32, 32}},
        GameConstants::TILE_SIZE,
        x, y);
         // initial texture rectangle

    // e.add<PlayerControlComponent>();
    return e;
}