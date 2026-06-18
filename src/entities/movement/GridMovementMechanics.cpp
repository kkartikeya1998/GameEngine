#include "entities/movement/GridMovementMechanics.h"

GridMovementMechanics::GridMovementMechanics(int x, int y, Direction dir)
    : position_{x, y, dir} {}

// -------------------------
// Position access
// -------------------------
Position GridMovementMechanics::getPosition() const
{
    return position_;
}

void GridMovementMechanics::setPosition(int x, int y)
{
    position_.x = x;
    position_.y = y;
}

void GridMovementMechanics::setDirection(Direction dir)
{
    position_.dir = dir;
}

Direction GridMovementMechanics::getDirection() const
{
    return position_.dir;
}

// -------------------------
// Movement logic
// -------------------------
Position GridMovementMechanics::nextPos(Direction dir) const
{
    Position next = position_;

    switch (dir)
    {
    case Direction::UP:
        next.y -= 1;
        break;
    case Direction::DOWN:
        next.y += 1;
        break;
    case Direction::LEFT:
        next.x -= 1;
        break;
    case Direction::RIGHT:
        next.x += 1;
        break;
    default:
        break;
    }

    return next;
}

void GridMovementMechanics::move(Direction dir)
{
    position_.dir = dir;

    Position next = nextPos(dir);
    position_.x = next.x;
    position_.y = next.y;
}
