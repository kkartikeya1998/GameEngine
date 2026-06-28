#pragma once

#include <memory>

#include "entities/movement/IMovementBehavior.h"
#include "render/IRenderState.h"

// ---------------------------------------------------------------------------
// Entity — anything on the map with a position, facing, and a way to be
// drawn: Player, Npc, and (in the future, once it needs to animate or be
// queried uniformly) MapObject.
//
// REPLACES the old Entity / GridEntity / FreeEntity / Player inheritance
// chain. There is now exactly ONE concrete Entity class. What used to be
// "which subclass do I pick" is now "which TWO components do I inject":
//
//   - IMovementBehavior: GridMovementMechanics or FreeMovementMechanics
//     (or a future third kind) — decides how position changes.
//   - IRenderState: AnimationComponentAdapter or WalkCycleTimerAdapter
//     (or a future SCML-backed kind) — decides how logical position
//     becomes a render position + animation progress.
//
// These two axes are now INDEPENDENT. A grid-moving NPC can use
// WalkCycleTimerAdapter, or a future SCML adapter, without anyone
// writing a new Entity subclass — this is the actual problem the old
// GridEntity/FreeEntity split could not solve (movement-family and
// render-family were welded together by which subclass you derived
// from).
//
// Player and Npc are no longer types — see Player.h (now a factory
// function) and Npc.h (now a thin wrapper holding an Entity plus
// NPC-only data, with the unique_ptr<Entity>-by-pointer indirection
// removed, since Entity itself can now represent any movement+render
// combination directly — no Liskov concern remains, since switching
// "type" was never actually needed at the Entity level once
// movement/render are swappable members rather than the class itself).
//
// update(dt) drives BOTH components every frame: movement first (so its
// resulting logical position is current), then render state (fed that
// fresh logical position + isMoving). This ordering matters — render
// state must always see THIS frame's already-resolved position, never
// last frame's.
// ---------------------------------------------------------------------------
class Entity {
public:
    Entity(std::unique_ptr<IMovementBehavior> movement,
           std::unique_ptr<IRenderState> renderState)
        : movement_(std::move(movement))
        , renderState_(std::move(renderState))
    {
    }

    // Called every frame, unconditionally, for every entity (Player,
    // every Npc). inputDir is NONE for any entity not currently being
    // driven by live input this frame — for Player that's "no key
    // held," for Npc today that's ALWAYS NONE (no behavior driver
    // exists yet — see Npc.h). isBlocked is built by the caller
    // (GameController) from the active Map.
    void update(float dt, Direction inputDir,
                const std::function<bool(const AABB&)>& isBlocked)
    {
        movement_->update(dt, inputDir, isBlocked);
        renderState_->update(dt, movement_->getX(), movement_->getY(), movement_->isMoving());
    }

    Direction getDirection() const { return movement_->getFacing(); }

    float getRenderX() const { return renderState_->getRenderX(); }
    float getRenderY() const { return renderState_->getRenderY(); }

    // Logical (non-interpolated) position — grid index as float for
    // grid entities, live continuous position for free entities. For
    // anything that needs the EXACT grid index (footprint/tile logic),
    // downcast via movement() to the concrete GridMovementMechanics and
    // call getGridX()/getGridY() instead — same escape-hatch pattern as
    // before.
    float getX() const { return movement_->getX(); }
    float getY() const { return movement_->getY(); }

    AABB getHitbox() const { return movement_->getHitbox(); }
    bool isMoving() const { return movement_->isMoving(); }

    // 0..1 progress for frame selection — meaning depends on which
    // IRenderState is active (see IRenderState.h). Replaces the old
    // split between Entity::getAnimationProgress() (grid) and
    // FreeEntity::getWalkProgress() (free) with one name.
    float getAnimProgress() const { return renderState_->getProgress(); }

    IMovementBehavior& movement() { return *movement_; }
    const IMovementBehavior& movement() const { return *movement_; }

    IRenderState& renderState() { return *renderState_; }
    const IRenderState& renderState() const { return *renderState_; }

private:
    std::unique_ptr<IMovementBehavior> movement_;
    std::unique_ptr<IRenderState> renderState_;
};