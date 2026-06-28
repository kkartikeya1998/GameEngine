#pragma once

#include "entities/movement/IMovementBehavior.h"

// ---------------------------------------------------------------------------
// FreeMovementMechanics — continuous position, axis-separated collision
// resolution.
//
// CHANGED: implements IMovementBehavior instead of the old
// IFreeMovementMechanics. NO BEHAVIOR CHANGE — this class already did
// exactly what the unified interface asks for (self-contained collision
// resolution via an injected isBlocked callback, called every frame
// regardless of whether dir is NONE). Only the interface name, and
// getPosition()/setPosition() being replaced by getX()/getY() (the
// uniform float read surface) plus a concrete-only setPosition() kept
// for teleport/spawn code, change here.
//
// hitboxWidth/hitboxHeight default smaller than a full tile on purpose —
// using the full sprite/tile size as the collision box makes movement
// feel like it's colliding with invisible walls around empty space near
// the sprite's edges. A hitbox roughly centered on the sprite's feet
// (e.g. half to two-thirds of TILE_SIZE) reads better. Tune visually once
// you can see it move.
// ---------------------------------------------------------------------------
class FreeMovementMechanics : public IMovementBehavior {
public:
    FreeMovementMechanics(float x, float y,
                           float speed,
                           float hitboxWidth, float hitboxHeight,
                           float hitboxOffsetX, float hitboxOffsetY,
                           Direction dir = Direction::NONE);

    void update(float dt, Direction inputDir,
                const std::function<bool(const AABB&)>& isBlocked) override;

    float getX() const override;
    float getY() const override;
    Direction getFacing() const override;
    AABB getHitbox() const override;
    bool isMoving() const override;

    // Concrete-only — teleport/spawn code that already knows it's
    // holding a FreeMovementMechanics (via Entity::movement() downcast)
    // can reposition directly. Not part of IMovementBehavior, same
    // reasoning as GridMovementMechanics::setGridPosition.
    void setPosition(float x, float y);

private:
    Position2D position_;
    float speed_;
    float hitboxWidth_;
    float hitboxHeight_;
    bool wasMoving_ = false;
    float offsetX_ = 0.f;
    float offsetY_ = 0.f;

    AABB hitboxAt(float x, float y) const;
};