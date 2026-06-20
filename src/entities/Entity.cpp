#include "entities/Entity.h"

Entity::Entity(std::unique_ptr<IMovementMechanics> movement)
    : movement_(std::move(movement)),
      animation_(6.0f)
{
}

void Entity::move(Direction dir)
{
    Position start = movement_->getPosition();

    movement_->move(dir);

    Position target = movement_->getPosition();

    animation_.begin(
        start.x,
        start.y,
        target.x,
        target.y
    );
}

void Entity::update(float dt)
{
    animation_.update(dt);
}

int Entity::getX() const
{
    return movement_->getPosition().x;
}

int Entity::getY() const
{
    return movement_->getPosition().y;
}

Direction Entity::getDirection() const
{
    return movement_->getDirection();
}

float Entity::getRenderX() const
{
    return animation_.isAnimating()
        ? animation_.getRenderX()
        : getX();
}

float Entity::getRenderY() const
{
    return animation_.isAnimating()
        ? animation_.getRenderY()
        : getY();
}

IMovementMechanics& Entity::movement()
{
    return *movement_;
}

const IMovementMechanics& Entity::movement() const
{
    return *movement_;
}

bool Entity::isAnimating() const
{
    return animation_.isAnimating();
}

float Entity::getAnimationProgress() const
{
    return animation_.getProgress();
}