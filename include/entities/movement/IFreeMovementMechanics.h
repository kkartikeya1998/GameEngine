#pragma once

#include <functional>

#include "entities/movement/Position2D.h"

// ---------------------------------------------------------------------------
// IFreeMovementMechanics — contract for continuous (non-grid-snapped)
// movement.
//
// Deliberately NOT a widening of IMovementMechanics. That interface's
// move(Direction)/nextPos(Direction)->Position shape is correct for grid
// movement (discrete, commit-or-refuse, no dt) and forcing it to also
// serve continuous movement would mean either lying about return types
// or smuggling dt through a side channel. Free movement gets its own
// interface; GridMovementMechanics and IMovementMechanics are untouched.
//
// Collision is injected as a callback (isBlocked) rather than this
// interface knowing about Map/GameController — same decoupling
// principle IMovementMechanics already uses (it never sees Map either;
// GameController checks walkability externally and decides whether to
// commit). Whoever owns the entity (GameController) is responsible for
// building isBlocked from the active Map + TILE_SIZE.
// ---------------------------------------------------------------------------
class IFreeMovementMechanics {
public:
    virtual ~IFreeMovementMechanics() = default;

    // Called once per frame. inputDir is the raw input this frame (NONE
    // if no movement key is held); dt is frame time. isBlocked(box)
    // returns true if the given world-space AABB overlaps something
    // solid. Resolves collision internally (axis-separated sliding) and
    // updates + returns the entity's new position.
    virtual Position2D update(float dt, Direction inputDir,
                               const std::function<bool(const AABB&)>& isBlocked) = 0;

    virtual Position2D getPosition() const = 0;
    virtual void setPosition(float x, float y) = 0;

    virtual Direction getFacing() const = 0;

    // The entity's own collision box, in world space, centered on its
    // current position. Exposed so callers (e.g. teleport-trigger
    // checks) can query overlap without duplicating hitbox geometry.
    virtual AABB getHitbox() const = 0;

    // True if input this frame resulted in actual movement (i.e. the
    // entity isn't fully blocked on both axes) — lets callers drive walk
    // animation off "is this entity currently moving" the same way
    // AnimationComponent currently keys off isAnimating() for grid moves.
    virtual bool isMoving() const = 0;
};