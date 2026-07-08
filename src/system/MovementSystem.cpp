#include "system/MovementSystem.h"
#include "system/GameConstants.h"

namespace MovementSystem
{

    void update(Entity &entity, const PlayerControlComponent &input, const Map &map,
                float dt, const std::function<bool(const AABB &)> &isBlocked)
    {

        auto *position = entity.get<PositionComponent>();
        auto *velocity = entity.get<VelocityComponent>();
        auto *direction = entity.get<DirectionComponent>();
        auto *movement = entity.get<FreeMovementComponent>();
        auto *collision = entity.get<CollisionComponent>();
        auto *state = entity.get<MovementStateComponent>();

        if (!position || !velocity || !direction || !movement || !collision || !state)
            return;

        auto inputDir = input.direction;
        auto sprintRequested = input.sprinting;
        float effectiveSpeed = movement->speed * (sprintRequested ? movement->sprintMultiplier : 1.f);

        velocity->vx = 0.f;
        velocity->vy = 0.f;

        switch (inputDir)
        {
        case Direction::UP:
            velocity->vy = -effectiveSpeed;
            break;
        case Direction::DOWN:
            velocity->vy = effectiveSpeed;
            break;
        case Direction::LEFT:
            velocity->vx = -effectiveSpeed;
            break;
        case Direction::RIGHT:
            velocity->vx = effectiveSpeed;
            break;
        case Direction::NONE:
            break;
        }

        if (inputDir != Direction::NONE)
            direction->facing = inputDir;

        bool moved = false;

        if (velocity->vx != 0.f)
        {
            float dx = velocity->vx * dt;
            AABB testX = collision->resolve(position->x + dx, position->y);
            if (!isBlocked(testX) && mapBoundsCheck(map, testX))
            {
                position->x += dx;
                moved = true;
            }
            else
                velocity->vx = 0.f;
        }

        if (velocity->vy != 0.f)
        {
            float dy = velocity->vy * dt;
            AABB testY = collision->resolve(position->x, position->y + dy);
            if (!isBlocked(testY) && mapBoundsCheck(map, testY))
            {
                position->y += dy;
                moved = true;
            }
            else
                velocity->vy = 0.f;
        }

        state->current = !moved ? MovementState::Idle
                                : (sprintRequested ? MovementState::Sprinting : MovementState::Walking);
    }

    bool mapBoundsCheck(const Map &map, const AABB &aabb)
    {
        return aabb.x >= 0.f && aabb.y >= 0.f &&
               aabb.x + aabb.width <= map.getWidth() * GameConstants::TILE_SIZE &&
               aabb.y + aabb.height <= map.getHeight() * GameConstants::TILE_SIZE;
    }
}
// namespace MovementSystem