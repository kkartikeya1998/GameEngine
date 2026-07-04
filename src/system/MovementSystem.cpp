#include "system/MovementSystem.h"
#include "system/GameConstants.h"

namespace MovementSystem {

// ---- Free movement ---------------------------------------------------

AABB freeHitboxAt(const FreeMovementComponent& movement, float x, float y)
{
    float boxBottomCenterX = x + movement.hitboxOffsetX;
    float boxBottomCenterY = y + movement.hitboxOffsetY;

    return AABB{
        boxBottomCenterX - movement.hitboxWidth  / 2.f,
        boxBottomCenterY - movement.hitboxHeight,
        movement.hitboxWidth,
        movement.hitboxHeight
    };
}

AABB getFreeHitbox(const PositionComponent& position, const FreeMovementComponent& movement)
{
    return freeHitboxAt(movement, position.x, position.y);
}

void updateFree(PositionComponent& position, VelocityComponent& velocity,
                DirectionComponent& direction, FreeMovementComponent& movement,
                float dt, Direction inputDir,
                const std::function<bool(const AABB&)>& isBlocked)
{
    // Compute desired velocity from input — same arithmetic as before,
    // now written to VelocityComponent instead of dying as local dx/dy.
    velocity.vx = 0.f;
    velocity.vy = 0.f;

    switch (inputDir) {
        case Direction::UP:    velocity.vy = -movement.speed * dt; break;
        case Direction::DOWN:  velocity.vy =  movement.speed * dt; break;
        case Direction::LEFT:  velocity.vx = -movement.speed * dt; break;
        case Direction::RIGHT: velocity.vx =  movement.speed * dt; break;
        case Direction::NONE:  break;
    }

    // Facing updates on any nonzero input before collision tests —
    // preserved exactly from original contract.
    if (inputDir != Direction::NONE)
        direction.facing = inputDir;   // was: position.facing = inputDir

    bool moved = false;

    if (velocity.vx != 0.f) {
        AABB testX = freeHitboxAt(movement, position.x + velocity.vx, position.y);
        if (!isBlocked(testX)) {
            position.x += velocity.vx;
            moved = true;
        } else {
            velocity.vx = 0.f; // blocked — zero out so callers see actual velocity
        }
    }

    if (velocity.vy != 0.f) {
        AABB testY = freeHitboxAt(movement, position.x, position.y + velocity.vy);
        if (!isBlocked(testY)) {
            position.y += velocity.vy;
            moved = true;
        } else {
            velocity.vy = 0.f; // blocked — zero out so callers see actual velocity
        }
    }

    movement.isMoving = moved;
}

} // namespace MovementSystem