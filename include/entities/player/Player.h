#pragma once

#include "entities/Entity.h"
#include "entities/movement/FreeMovementComponent.h"
#include "render/FreeRenderComponent.h"
#include "entities/player/PlayerControlComponent.h"

// ---------------------------------------------------------------------------
// makePlayer — builds the one player-controlled Entity.
//
// REPLACES `using Player = Entity;`. There is no more Player TYPE —
// only this factory, which builds a plain Entity carrying:
//   - FreeMovementComponent  (position, speed, hitbox, facing)
//   - FreeRenderComponent    (walk-cycle render state)
//   - PlayerControlComponent (empty tag — marks this as the player)
//
// Everywhere that used to take/return `Player&` or `Player*` now takes/
// returns `Entity&`/`Entity*` — GameController::getPlayer() included.
// The PlayerControlComponent tag is what lets code that genuinely needs
// to distinguish "the player" from "an NPC" do so
// (entity.has<PlayerControlComponent>()), without a separate type.
//
// Signature matches the values GameController::GameController already
// passes in (see GameController.cpp): x, y in pixel space, speed and
// hitbox dimensions from GameConstants. No movement-strategy object is
// injected anymore (FreeMovementMechanics is gone) — this just
// populates a FreeMovementComponent's fields directly.
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

    e.add<FreeMovementComponent>(FreeMovementComponent{
        x, y, dir,
        speed,
        hitboxWidth, hitboxHeight,
        hitboxOffsetX, hitboxOffsetY,
        false // isMoving
    });

    e.add<FreeRenderComponent>(FreeRenderComponent{
        walkCyclesPerSecond,
        0.f,  // elapsed
        x, y  // renderX/Y start at spawn position
    });

    e.add<PlayerControlComponent>();

    return e;
}