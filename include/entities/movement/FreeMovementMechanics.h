#pragma once

#include "entities/movement/IFreeMovementMechanics.h"

// ---------------------------------------------------------------------------
// FreeMovementMechanics — continuous position, axis-separated collision
// resolution.
//
// hitboxWidth/hitboxHeight default smaller than a full tile on purpose —
// using the full sprite/tile size as the collision box makes movement
// feel like it's colliding with invisible walls around empty space near
// the sprite's edges. A hitbox roughly centered on the sprite's feet
// (e.g. half to two-thirds of TILE_SIZE) reads better. Tune visually once
// you can see it move.
// ---------------------------------------------------------------------------
class FreeMovementMechanics : public IFreeMovementMechanics {
public:
    FreeMovementMechanics(float x, float y,
                           float speed,
                           float hitboxWidth, float hitboxHeight,
                           Direction dir = Direction::NONE);

    Position2D update(float dt, Direction inputDir,
                       const std::function<bool(const AABB&)>& isBlocked) override;

    Position2D getPosition() const override;
    void setPosition(float x, float y) override;

    Direction getFacing() const override;

    AABB getHitbox() const override;

    bool isMoving() const override;

private:
    Position2D position_;
    float speed_;
    float hitboxWidth_;
    float hitboxHeight_;
    bool wasMoving_ = false;

    AABB hitboxAt(float x, float y) const;
};