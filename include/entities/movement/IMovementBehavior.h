#pragma once

#include <functional>

#include "entities/movement/Position2D.h"

// ---------------------------------------------------------------------------
// IMovementBehavior — unified contract for ALL movement, grid or free.
//
// REPLACES IMovementMechanics and IFreeMovementMechanics. Those two
// existed separately because grid movement was commit-or-refuse
// (move(Direction) -> Position, decided externally by GameController
// checking Map::isAreaBlocked first) while free movement was
// continuous and self-resolving (update(dt, dir, isBlocked) decides
// collision internally). That asymmetry is gone now: BOTH concrete
// mechanics take the same update() shape and BOTH own their own
// collision check internally via the injected isBlocked callback.
//
// WHY THIS CHANGE (not just a signature widen): GameController
// previously had to know how to validate a grid move against the map
// before calling move(dir). That's a responsibility no caller should
// need — an NPC behavior driver (future) wants to say "try to walk
// this way" and trust the mechanic to refuse if blocked, exactly like
// free movement already works for Player. Centralizing collision
// resolution inside each mechanic is what makes Entity uniform across
// Player AND Npc, and is what makes a future NPC AI/behavior_ driver
// possible without it re-implementing Map-aware collision logic.
//
// isMoving() reports whether THIS frame's update() actually changed
// position (free movement: true if not fully blocked on both axes;
// grid movement: true only on the frame a hop is committed — see
// GridMovementMechanics::update for exactly what "this frame" means
// for a discrete mechanic).
//
// getX()/getY() return float for both. For grid movement this is
// always a whole number representing the current (or just-departed,
// pre-animation) tile — Entity/IRenderState consumes it the same way
// regardless of which mechanic produced it. Grid-specific integer tile
// access (needed by Map::tile_at, footprint logic, etc.) stays on the
// CONCRETE class (GridMovementMechanics::getGridPosition() or similar)
// — it is deliberately NOT part of this interface, the same way
// setPosition() isn't: teleport/spawn code that needs to set a precise
// position already knows its concrete mechanic type (it's the code
// that constructed the Entity in the first place) and can downcast via
// Entity::movement(), same escape-hatch pattern Npc::current() already
// uses for Entity itself.
// ---------------------------------------------------------------------------
class IMovementBehavior {
public:
    virtual ~IMovementBehavior() = default;

    // Called every frame, unconditionally — same calling convention for
    // every entity (Player, Npc, anything else), same as FreeEntity's
    // updateMovement already worked. inputDir is this frame's requested
    // direction (NONE = no movement requested). dt is frame time.
    // isBlocked(box) tests a world-space AABB against the active map +
    // any other solid entities; built by the caller (GameController)
    // from Map::isAreaBlocked, same as before.
    //
    // Free movement: resolves continuously, per-axis, every frame (no
    // change from FreeMovementMechanics::update's existing behavior).
    //
    // Grid movement: on a frame where inputDir != NONE and the mechanic
    // is not already mid-hop, attempts ONE discrete hop — tests the
    // destination tile via isBlocked, commits position only if clear.
    // On frames where inputDir == NONE, or where a hop is already
    // committed and only its visual interpolation is in flight, this is
    // a no-op with respect to position (the visual lerp itself is
    // IRenderState's concern now, not this interface's).
    virtual void update(float dt, Direction inputDir,
                         const std::function<bool(const AABB&)>& isBlocked) = 0;

    virtual float getX() const = 0;
    virtual float getY() const = 0;

    virtual Direction getFacing() const = 0;

    // This entity's own collision box in world space, at its CURRENT
    // (already-committed) position. Same role as the old
    // IFreeMovementMechanics::getHitbox() — used by teleport-trigger
    // checks etc. without duplicating hitbox geometry per caller.
    virtual AABB getHitbox() const = 0;

    // True if this frame's update() call actually changed position.
    virtual bool isMoving() const = 0;
};