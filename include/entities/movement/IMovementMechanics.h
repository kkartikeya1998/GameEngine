#pragma once

#include <memory>
#include <vector>

#include "entities/movement/Position.h"

struct MoveResult {
    int dx = 0;
    int dy = 0;
    int speed = 1;
};

class IMovementMechanics {
public:
    virtual ~IMovementMechanics() = default;

    virtual Position getPosition() const = 0;

    virtual void setPosition(int x, int y) = 0;
    virtual void setDirection(Direction dir) = 0;

    virtual Direction getDirection() const = 0;

    virtual void move(Direction dir) = 0;
    virtual Position nextPos(Direction dir) const = 0;
};