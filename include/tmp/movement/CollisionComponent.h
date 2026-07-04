#pragma once

#include "entities/Component.h"
#include "tmp/movement/PositionComponent.h" // for AABB

// ---------------------------------------------------------------------------
// CollisionComponent — owns an entity's collidable rectangle.
// // TODO: Maybe make it a variant to allow strager shapes
// ---------------------------------------------------------------------------
struct CollisionComponent : public Component {
    float offsetX = 0.f;
    float offsetY = 0.f;
    float width = 0.f;
    float height = 0.f;

    CollisionComponent() = default;
    CollisionComponent(float offsetX, float offsetY, float width, float height)
        : offsetX(offsetX), offsetY(offsetY), width(width), height(height)
    {}

    // make-do converter for AABB component in debug rendering
    // worldX/worldY = the entity's PositionComponent.
    AABB resolve(float worldX, float worldY) const {
        return AABB{
            worldX - width / 2.f + offsetX,
            worldY - height + offsetY,
            width,
            height
        };
    }
};