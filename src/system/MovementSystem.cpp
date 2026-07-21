#include "system/MovementSystem.h"
#include "system/GameConstants.h"
#include <algorithm>

namespace MovementSystem
{

    void update(Registry &registry, EntityID id, const Map &map, float dt,
                const std::function<bool(const AABB &)> &isBlocked,
                const PlayerControlComponent *input)
    {
        dt = std::min(dt, kMaxFrameDt);

        auto *position = registry.get<PositionComponent>(id);
        auto *velocity = registry.get<VelocityComponent>(id);
        auto *direction = registry.get<DirectionComponent>(id);
        auto *state = registry.get<MovementStateComponent>(id);

        if (!position || !velocity || !direction || !state)
            return;

        bool sprintRequested = false;

        if (input)
        {
            auto *movement = registry.get<FreeMovementComponent>(id);
            if (!movement)
                return;

            sprintRequested = input->sprinting;
            float effectiveSpeed = movement->speed * (sprintRequested ? movement->sprintMultiplier : 1.f);

            velocity->vx = 0.f;
            velocity->vy = 0.f;

            switch (input->direction)
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

            if (input->direction != Direction::NONE)
                direction->facing = input->direction;
        }

        auto *collision = registry.get<CollisionComponent>(id);
        bool moved = false;

        if (velocity->vx != 0.f)
        {
            float dx = velocity->vx * dt;
            AABB testX = collision ? collision->resolve(position->x + dx, position->y)
                                   : AABB{position->x + dx, position->y, 0.f, 0.f};
            if (!isBlocked(testX) && mapBoundsCheck(map, testX) && map.isWalkable(testX))
            {
                position->x += dx;
                moved = true;
            }
            else if (input)
                velocity->vx = 0.f;
        }

        if (velocity->vy != 0.f)
        {
            float dy = velocity->vy * dt;
            AABB testY = collision ? collision->resolve(position->x, position->y + dy)
                                   : AABB{position->x, position->y + dy, 0.f, 0.f};
            if (!isBlocked(testY) && mapBoundsCheck(map, testY) && map.isWalkable(testY))
            {
                position->y += dy;
                moved = true;
            }
            else if (input)
                velocity->vy = 0.f;
        }

        if (input)
        {
            state->current = !moved ? MovementState::Idle
                                    : (sprintRequested ? MovementState::Sprinting : MovementState::Walking);
        }
    }

    bool mapBoundsCheck(const Map &map, const AABB &aabb)
    {
        return aabb.x >= 0.f && aabb.y >= 0.f &&
               aabb.x + aabb.width <= map.getWidth() * GameConstants::TILE_SIZE &&
               aabb.y + aabb.height <= map.getHeight() * GameConstants::TILE_SIZE;
    }
}