#pragma once

#include "entities/Component.h"

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

struct DirectionComponent : public Component {
    Direction facing = Direction::NONE;

    DirectionComponent() = default;
    explicit DirectionComponent(Direction facing) : facing(facing) {}
};
