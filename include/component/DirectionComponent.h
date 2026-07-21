#pragma once

#include "entities/Component.h"

enum class Direction {
    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT,
    NONE
};

struct DirectionComponent : public Component {
    Direction facing = Direction::NONE;

    DirectionComponent() = default;
    explicit DirectionComponent(Direction facing) : facing(facing) {}
};
