#include "tmp/movement/FreeMovementMechanics.h"

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
    float boxBottomCenterX = x + offsetX_;
    float boxBottomCenterY = y + offsetY_;

    return AABB{
        boxBottomCenterX - hitboxWidth_ / 2.f,
        boxBottomCenterY - hitboxHeight_,
        hitboxWidth_,
        hitboxHeight_
    };
}

void FreeMovementMechanics::update(float dt, Direction inputDir,
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
}

float FreeMovementMechanics::getX() const { return position_.x; }
float FreeMovementMechanics::getY() const { return position_.y; }
Direction FreeMovementMechanics::getFacing() const { return position_.dir; }
bool FreeMovementMechanics::isMoving() const { return wasMoving_; }

AABB FreeMovementMechanics::getHitbox() const
{
    return hitboxAt(position_.x, position_.y);
}

void FreeMovementMechanics::setPosition(float x, float y)
{
    position_.x = x;
    position_.y = y;
}