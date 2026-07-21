#pragma once

#include "entities/Registry.h"

#include "component/VelocityComponent.h"
#include "component/DirectionComponent.h"

#include <cmath>

namespace DirectionSystem
{
    // Clockwise from RIGHT (atan2 angle 0), matching screen coords where +y is down.
    constexpr std::array<Direction, 8> kOctant = {
        Direction::RIGHT, Direction::DOWN_RIGHT, Direction::DOWN, Direction::DOWN_LEFT,
        Direction::LEFT, Direction::UP_LEFT, Direction::UP, Direction::UP_RIGHT};

    void update(Registry &registry)
    {
        for (EntityID id : registry.view<VelocityComponent, DirectionComponent>())
        {
            auto *velocity = registry.get<VelocityComponent>(id);
            auto *direction = registry.get<DirectionComponent>(id);

            if (!velocity || !direction)
                continue;

            float vx = velocity->vx;
            float vy = velocity->vy;

            if (vx == 0.f && vy == 0.f)
                continue;

            float degrees = std::atan2(vy, vx) * (180.f / 3.14159265f);
            if (degrees < 0.f)
                degrees += 360.f;

            int octant = static_cast<int>((degrees + 22.5f) / 45.f) % 8;
            direction->facing = kOctant[octant];
        }
    }

}