#pragma once

#include "entities/Entity.h"
#include "tmp/movement/PositionComponent.h"
#include "tmp/movement/VelocityComponent.h"
#include "tmp/movement/DirectionComponent.h"
#include "tmp/movement/FreeMovementComponent.h"
#include "render/FreeRenderComponent.h"
#include "entities/player/PlayerControlComponent.h"
#include "system/GameConstants.h"

inline Entity makePlayer(
    float x, float y,
    float movement_speed = GameConstants::PLAYER_SPEED,
    float walkCyclesPerSecond = 4.0f)
{
    Entity e;
    e.add<PositionComponent>(x, y);
    e.add<VelocityComponent>();
    e.add<DirectionComponent>(Direction::DOWN);
    e.add<FreeMovementComponent>(
        movement_speed,
        GameConstants::PLAYER_HITBOX_WIDTH,
        GameConstants::PLAYER_HITBOX_HEIGHT,
        GameConstants::PLAYER_HITBOX_OFFSET_X,
        GameConstants::PLAYER_HITBOX_OFFSET_Y, false);
    e.add<FreeRenderComponent>(walkCyclesPerSecond, 0.f, x, y);
    e.add<PlayerControlComponent>();
    return e;
}