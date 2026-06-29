#pragma once

#include "entities/Entity.h"
#include "entities/movement/PositionComponent.h"
#include "entities/movement/FreeMovementComponent.h"
#include "render/FreeRenderComponent.h"
#include "entities/player/PlayerControlComponent.h"

// ---------------------------------------------------------------------------
// makePlayer — builds the one player-controlled Entity.
//
// CHANGED (PositionComponent pass): builds a PositionComponent
// alongside FreeMovementComponent now that FreeMovementComponent no
// longer carries x/y/facing itself. Spawn x/y/dir go onto
// PositionComponent; FreeMovementComponent gets only the
// movement-specific fields (speed, hitbox*).
//
// Builds a plain Entity carrying:
//   - PositionComponent      (x, y, facing — NEW, see above)
//   - FreeMovementComponent  (speed, hitbox)
//   - FreeRenderComponent    (walk-cycle render state)
//   - PlayerControlComponent (empty tag — marks this as the player)
//
// Everywhere that used to take/return `Player&` or `Player*` now takes/
// returns `Entity&`/`Entity*` — GameController::getPlayer() included.
// ---------------------------------------------------------------------------
inline Entity makePlayer(
    float x, float y,
    float speed,
    float hitboxWidth, float hitboxHeight,
    float hitboxOffsetX, float hitboxOffsetY,
    float walkCyclesPerSecond = 4.0f,
    Direction dir = Direction::NONE)
{
    Entity e;

    e.add<PositionComponent>(x, y, dir);

    e.add<FreeMovementComponent>(
        speed,
        hitboxWidth, hitboxHeight,
        hitboxOffsetX, hitboxOffsetY,
        false // isMoving
    );

    e.add<FreeRenderComponent>(
        walkCyclesPerSecond,
        0.f,  // elapsed
        x, y  // renderX/Y start at spawn position
    );

    e.add<PlayerControlComponent>();

    return e;
}