#include "entities/movement/FreeMovementMechanics.h"
#include <iostream>

FreeMovementMechanics::FreeMovementMechanics(float x, float y,
                                              float speed,
                                              float hitboxWidth, float hitboxHeight,
                                              float offsetX, float offsetY,
                                              Direction dir)
    : position_{x, y, dir}
    , speed_(speed)
    , hitboxWidth_(hitboxWidth)
    , hitboxHeight_(hitboxHeight)
    , offsetX_(offsetX)
    , offsetY_(offsetY)
{
}


AABB FreeMovementMechanics::hitboxAt(float x, float y) const
{
    // offsetX_/offsetY_ displace the box's bottom-center from (x, y) —
    // same role as ObjectTypeMetadata::CollisionBox's offsetX/offsetY
    // displacing from an object's origin. Default {0,0} reproduces the
    // exact previous behavior (box bottom-center == position) for every
    // existing caller.
    float boxBottomCenterX = x + offsetX_;
    float boxBottomCenterY = y + offsetY_;

    return AABB{
        boxBottomCenterX - hitboxWidth_ / 2.f,
        boxBottomCenterY - hitboxHeight_,
        hitboxWidth_,
        hitboxHeight_
    };
}
Position2D FreeMovementMechanics::update(float dt, Direction inputDir,
                                          const std::function<bool(const AABB&)>& isBlocked)
{
    float dx = 0.f, dy = 0.f;

    switch (inputDir) {
        case Direction::UP:    dy = -speed_ * dt; break;
        case Direction::DOWN:  dy =  speed_ * dt; break;
        case Direction::LEFT:  dx = -speed_ * dt; break;
        case Direction::RIGHT: dx =  speed_ * dt; break;
        case Direction::NONE:  break;
    }

    if (inputDir != Direction::NONE) {
        position_.dir = inputDir;
    }

    bool moved = false;

    // Resolve X and Y independently so the entity slides along an
    // obstruction instead of stopping outright when only one axis is
    // blocked (e.g. walking diagonally into a wall corner).
    if (dx != 0.f) {
        AABB testX = hitboxAt(position_.x + dx, position_.y);
        if (!isBlocked(testX)) {
            position_.x += dx;
            moved = true;
        }
    }

    if (dy != 0.f) {
        AABB testY = hitboxAt(position_.x, position_.y + dy);
        if (!isBlocked(testY)) {
            position_.y += dy;
            moved = true;
        }
    }
    
    wasMoving_ = moved;
    return position_;
}

Position2D FreeMovementMechanics::getPosition() const
{
    return position_;
}

void FreeMovementMechanics::setPosition(float x, float y)
{
    position_.x = x;
    position_.y = y;
}

Direction FreeMovementMechanics::getFacing() const
{
    return position_.dir;
}

AABB FreeMovementMechanics::getHitbox() const
{
    return hitboxAt(position_.x, position_.y);
}

bool FreeMovementMechanics::isMoving() const
{
    return wasMoving_;
}