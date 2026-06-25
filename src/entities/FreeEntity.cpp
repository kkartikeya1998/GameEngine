#include "entities/FreeEntity.h"

FreeEntity::FreeEntity(std::unique_ptr<IFreeMovementMechanics> movement,
                        float walkCyclesPerSecond)
    : movement_(std::move(movement))
    , walkCycle_(walkCyclesPerSecond)
{
}

void FreeEntity::updateMovement(float dt, Direction inputDir,
                                 const std::function<bool(const AABB&)>& isBlocked)
{
    movement_->update(dt, inputDir, isBlocked);
}

void FreeEntity::update(float dt)
{
    // Deliberately does NOT call Entity::update(dt) — that would drive
    // animation_ (AnimationComponent), which FreeEntity never uses.
    walkCycle_.update(dt, movement_->isMoving());
}

float FreeEntity::getX() const
{
    return movement_->getPosition().x;
}

float FreeEntity::getY() const
{
    return movement_->getPosition().y;
}

Direction FreeEntity::getDirection() const
{
    return movement_->getFacing();
}

float FreeEntity::getRenderX() const
{
    // No interpolation needed — position is already continuous and live.
    return movement_->getPosition().x;
}

float FreeEntity::getRenderY() const
{
    return movement_->getPosition().y;
}

AABB FreeEntity::getHitbox() const
{
    return movement_->getHitbox();
}

bool FreeEntity::isMoving() const
{
    return movement_->isMoving();
}

float FreeEntity::getWalkProgress() const
{
    return walkCycle_.getProgress();
}

IFreeMovementMechanics& FreeEntity::movement()
{
    return *movement_;
}

const IFreeMovementMechanics& FreeEntity::movement() const
{
    return *movement_;
}