#pragma once

#include "entities/Component.h"
#include "entities/movement/Position.h"

struct PositionComponent : public Component {
    float x = 0.f;
    float y = 0.f;
    Direction facing = Direction::NONE;
 
    PositionComponent() = default;
 
    PositionComponent(float x, float y, Direction facing = Direction::NONE)
        : x(x), y(y), facing(facing)
    {}
};