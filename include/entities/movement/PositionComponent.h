#pragma once
#include "entities/Component.h"

struct PositionComponent : public Component {
    float x = 0.f;
    float y = 0.f;

    PositionComponent() = default;
    PositionComponent(float x, float y) : x(x), y(y) {}
};

// temporarily here
struct AABB {
    float x, y; 
    float width, height;

    bool intersects(const AABB& other) const {
        return x < other.x + other.width  && x + width  > other.x &&
               y < other.y + other.height && y + height > other.y;
    }
};