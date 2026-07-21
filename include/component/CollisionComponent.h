#pragma once

#include "entities/Component.h"
#include "component/PositionComponent.h"

struct CollisionComponent : public Component {
    float offsetX = 0.f;
    float offsetY = 0.f;
    float width = 0.f;
    float height = 0.f;
    bool solid = true; // false = trigger-only (pickups, warps) — never blocks movement

    CollisionComponent() = default;
    CollisionComponent(float offsetX, float offsetY, float width, float height, bool solid = true)
        : offsetX(offsetX), offsetY(offsetY), width(width), height(height), solid(solid)
    {}

    AABB resolve(float worldX, float worldY) const {
        return AABB{
            worldX - width / 2.f + offsetX,
            worldY - height + offsetY,
            width,
            height
        };
    }
};