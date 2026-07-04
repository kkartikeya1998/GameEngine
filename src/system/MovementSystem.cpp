#include "system/MovementSystem.h"
#include "system/GameConstants.h"

namespace MovementSystem {

void update(PositionComponent& position, VelocityComponent& velocity,
                DirectionComponent& direction, FreeMovementComponent& movement,
                CollisionComponent& collision, MovementStateComponent& state,
                float dt, Direction inputDir, bool sprintRequested,
                const std::function<bool(const AABB&)>& isBlocked)
{
    float effectiveSpeed = movement.speed * (sprintRequested ? movement.sprintMultiplier : 1.f);

    velocity.vx = 0.f;
    velocity.vy = 0.f;

    switch (inputDir) {
        case Direction::UP:    velocity.vy = -effectiveSpeed; break;
        case Direction::DOWN:  velocity.vy =  effectiveSpeed; break;
        case Direction::LEFT:  velocity.vx = -effectiveSpeed; break;
        case Direction::RIGHT: velocity.vx =  effectiveSpeed; break;
        case Direction::NONE:  break;
    }

    if (inputDir != Direction::NONE)
        direction.facing = inputDir;

    bool moved = false;

    if (velocity.vx != 0.f) {
        float dx = velocity.vx * dt;
        AABB testX = collision.resolve(position.x + dx, position.y);
        if (!isBlocked(testX)) { position.x += dx; moved = true; }
        else velocity.vx = 0.f;
    }

    if (velocity.vy != 0.f) {
        float dy = velocity.vy * dt;
        AABB testY = collision.resolve(position.x, position.y + dy);
        if (!isBlocked(testY)) { position.y += dy; moved = true; }
        else velocity.vy = 0.f;
    }

    state.current = !moved ? MovementState::Idle
                  : (sprintRequested ? MovementState::Sprinting : MovementState::Walking);
}

} // namespace MovementSystem