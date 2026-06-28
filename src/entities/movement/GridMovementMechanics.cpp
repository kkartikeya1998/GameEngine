#include "entities/movement/GridMovementMechanics.h"
#include "system/GameConstants.h"

GridMovementMechanics::GridMovementMechanics(int x, int y, Direction dir)
    : position_{x, y, dir}
{
}

Position GridMovementMechanics::nextPos(Direction dir) const
{
    Position next = position_;
    switch (dir)
    {
    case Direction::UP:    next.y -= 1; break;
    case Direction::DOWN:  next.y += 1; break;
    case Direction::LEFT:  next.x -= 1; break;
    case Direction::RIGHT: next.x += 1; break;
    default: break;
    }
    return next;
}

AABB GridMovementMechanics::tileBoxAt(int gridX, int gridY) const
{
    return AABB{
        static_cast<float>(gridX) * GameConstants::TILE_SIZE,
        static_cast<float>(gridY) * GameConstants::TILE_SIZE,
        static_cast<float>(GameConstants::TILE_SIZE),
        static_cast<float>(GameConstants::TILE_SIZE)
    };
}

void GridMovementMechanics::update(float /*dt*/, Direction inputDir,
                                    const std::function<bool(const AABB&)>& isBlocked)
{
    // dt is unused: a grid hop is logically instantaneous (the visual
    // lerp over time is IRenderState's job, not this mechanic's). dt
    // stays a parameter only to satisfy IMovementBehavior's uniform
    // signature, exactly as documented on the interface.

    wasMoving_ = false;

    // Edge-trigger gate: only attempt a hop when inputDir is a genuinely
    // NEW non-NONE direction versus what we last acted on. Releasing the
    // key (NONE) clears the gate so the same direction can re-trigger
    // later. This is what keeps "held key" from spamming one hop per
    // frame now that update() is called unconditionally every frame.
    if (inputDir == Direction::NONE) {
        lastInputDir_ = Direction::NONE;
        return;
    }

    if (inputDir == lastInputDir_) {
        // Same direction still held from the frame that already
        // triggered a hop — do nothing until it's released.
        return;
    }

    lastInputDir_ = inputDir;

    Position next = nextPos(inputDir);
    AABB destBox = tileBoxAt(next.x, next.y);

    if (isBlocked(destBox)) {
        // Refused. Facing is NOT updated — matches the original
        // move(Direction)'s actual contract exactly: GameController
        // only ever called move() AFTER deciding the move was allowed,
        // so dir and position always changed together or not at all.
        // No "face the wall" behavior is introduced here.
        return;
    }

    position_.dir = inputDir;
    position_.x = next.x;
    position_.y = next.y;
    wasMoving_ = true;
}

float GridMovementMechanics::getX() const { return static_cast<float>(position_.x); }
float GridMovementMechanics::getY() const { return static_cast<float>(position_.y); }
Direction GridMovementMechanics::getFacing() const { return position_.dir; }
bool GridMovementMechanics::isMoving() const { return wasMoving_; }

AABB GridMovementMechanics::getHitbox() const
{
    return tileBoxAt(position_.x, position_.y);
}

void GridMovementMechanics::setGridPosition(int x, int y)
{
    position_.x = x;
    position_.y = y;
}