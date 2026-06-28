#pragma once

#include "entities/movement/IMovementBehavior.h"

// ---------------------------------------------------------------------------
// GridMovementMechanics — discrete, tile-snapped movement.
//
// CHANGED to implement IMovementBehavior instead of IMovementMechanics.
// Two real behavior changes versus before (not just a rename):
//
//   1. COLLISION CHECK MOVED IN. update() now takes isBlocked and tests
//      the destination tile itself before committing — the caller
//      (GameController) no longer validates a move before requesting
//      it. The box tested is exactly one TILE_SIZE x TILE_SIZE rect at
//      the destination grid cell — the SAME box Map::isAreaBlocked
//      already builds internally for its terrain-walkability pass — so
//      this reproduces prior behavior exactly, just invoked from the
//      other side of the call. On a refused move, NEITHER position NOR
//      facing change — same all-or-nothing contract the old
//      move(Direction) had (GameController only ever called move()
//      after already deciding to allow it, so position and facing
//      always moved together before; that's preserved here, just
//      decided internally now instead of externally).
//
//   2. EDGE-TRIGGERED, NOT LEVEL-TRIGGERED. The old move(Direction) was
//      only ever called once per accepted input (GameController decided
//      when to call it). Now that update() is called every frame
//      unconditionally (same calling convention as free movement), a
//      held key would otherwise cause a hop on every single frame
//      forever. To preserve grid movement's actual "one tile per press"
//      feel, this class tracks the direction it last acted on
//      (lastInputDir_) and only attempts a new hop when inputDir
//      changes to a new, non-NONE direction. Releasing the key (NONE)
//      clears the gate, so pressing the same direction again later
//      triggers a fresh hop. This is the ONE piece of policy that has
//      no equivalent in free movement (which has no concept of "edge"
//      at all) — it lives here, internal to the mechanic, rather than
//      leaking back out to GameController or Entity.
//
// NO hitbox parameter was added to the constructor — grid movement never
// had a configurable hitbox before (IMovementMechanics had no
// getHitbox() at all) and this isn't the place to invent one. getHitbox()
// below simply returns the same destination-tile rect isBlocked() is
// tested against, in WORLD-PIXEL space, for the few callers (teleport
// triggers, debug draw) that want it via the uniform IMovementBehavior
// surface. GameConstants::TILE_SIZE is used directly, same constant
// Map::isAreaBlocked already keys off.
//
// getGridX()/getGridY() (integer) are kept as concrete-only accessors —
// Map::tile_at, footprint logic, and anything genuinely grid-index-based
// still wants ints. getX()/getY() (the IMovementBehavior override,
// float) simply widen these for the uniform Entity-level contract.
// ---------------------------------------------------------------------------
class GridMovementMechanics : public IMovementBehavior {
public:
    explicit GridMovementMechanics(int x, int y, Direction dir = Direction::NONE);

    // -------------------------
    // IMovementBehavior
    // -------------------------
    void update(float dt, Direction inputDir,
                const std::function<bool(const AABB&)>& isBlocked) override;

    float getX() const override;
    float getY() const override;
    Direction getFacing() const override;
    AABB getHitbox() const override;
    bool isMoving() const override;

    // -------------------------
    // Concrete-only, grid-specific access (teleport/spawn, tile logic)
    // -------------------------
    int getGridX() const { return position_.x; }
    int getGridY() const { return position_.y; }
    void setGridPosition(int x, int y);

private:
    Position position_;          // grid-space, integer
    Direction lastInputDir_ = Direction::NONE;  // edge-trigger gate
    bool wasMoving_ = false;     // true only on the frame a hop committed

    Position nextPos(Direction dir) const;

    // World-pixel-space TILE_SIZE x TILE_SIZE rect at a given grid cell —
    // identical box shape Map::isAreaBlocked's own terrain pass uses.
    AABB tileBoxAt(int gridX, int gridY) const;
};