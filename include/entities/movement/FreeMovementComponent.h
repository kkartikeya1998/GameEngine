#pragma once

#include "entities/Component.h"
#include "entities/movement/Position2D.h"

// ---------------------------------------------------------------------------
// FreeMovementComponent — pure data for continuous, axis-separated
// movement.
//
// CHANGED (Component base pass): now inherits `: public Component` so
// it can live in Entity's vector<unique_ptr<Component>> storage. No
// field changes — see Component.h for why this base exists.
//
// REPLACES FreeMovementMechanics as a class. The logic that used to
// live in FreeMovementMechanics::update() (per-axis collision-tested
// displacement, facing update on nonzero input) now lives in
// MovementSystem::updateFree() — this struct only holds the state that
// logic reads and writes.
//
// Field-for-field, this is FreeMovementMechanics' old private section:
//   position_ (x, y, dir)                  -> x, y, facing
//   speed_                                  -> speed
//   hitboxWidth_/hitboxHeight_              -> hitboxWidth/hitboxHeight
//   offsetX_/offsetY_                       -> hitboxOffsetX/hitboxOffsetY
//   wasMoving_                              -> isMoving
//
// hitboxAt(x, y) (the box-construction helper) is NOT data and moves
// into MovementSystem as a free function — see MovementSystem.h.
// ---------------------------------------------------------------------------
// CAVEAT (Component base pass, confirmed by Player.h's build failure):
// Component's virtual destructor disqualifies this struct from
// aggregate-init, same issue already fixed once for
// MapObjectRenderComponent and missed here the first time around —
// Player.h's makePlayer() brace-initializes this type with 9
// positional args, which silently kept compiling as default-init +
// ignored args until the base class changed. Added the constructor
// below so that call site becomes a normal constructor call instead.
struct FreeMovementComponent : public Component {
    float x = 0.f;
    float y = 0.f;
    Direction facing = Direction::NONE;

    float speed = 0.f;

    // Hitbox sized smaller than a full tile on purpose — see original
    // FreeMovementMechanics.h comment: using full sprite/tile size as
    // the collision box makes movement feel like it collides with
    // invisible walls around empty space near the sprite's edges.
    float hitboxWidth = 0.f;
    float hitboxHeight = 0.f;
    float hitboxOffsetX = 0.f;
    float hitboxOffsetY = 0.f;

    // True only on a frame where at least one axis actually moved
    // (i.e. not fully blocked on both axes).
    bool isMoving = false;

    FreeMovementComponent() = default;

    FreeMovementComponent(
        float x, float y, Direction facing,
        float speed,
        float hitboxWidth, float hitboxHeight,
        float hitboxOffsetX, float hitboxOffsetY,
        bool isMoving = false
    )
        : x(x), y(y), facing(facing)
        , speed(speed)
        , hitboxWidth(hitboxWidth), hitboxHeight(hitboxHeight)
        , hitboxOffsetX(hitboxOffsetX), hitboxOffsetY(hitboxOffsetY)
        , isMoving(isMoving)
    {}
};