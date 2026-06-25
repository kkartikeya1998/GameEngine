#include "entities/GridEntity.h"

GridEntity::GridEntity(std::unique_ptr<IMovementMechanics> movement)
    : movement_(std::move(movement))
{
}

void GridEntity::move(Direction dir)
{
    Position start = movement_->getPosition();

    movement_->move(dir);

    Position target = movement_->getPosition();

    animation_.begin(
        static_cast<float>(start.x),
        static_cast<float>(start.y),
        static_cast<float>(target.x),
        static_cast<float>(target.y)
    );
}

int GridEntity::getX() const
{
    return movement_->getPosition().x;
}

int GridEntity::getY() const
{
    return movement_->getPosition().y;
}

Direction GridEntity::getDirection() const
{
    return movement_->getDirection();
}

float GridEntity::getRenderX() const
{
    return animation_.isAnimating()
        ? animation_.getRenderX()
        : static_cast<float>(getX());
}

float GridEntity::getRenderY() const
{
    return animation_.isAnimating()
        ? animation_.getRenderY()
        : static_cast<float>(getY());
}

IMovementMechanics& GridEntity::movement()
{
    return *movement_;
}

const IMovementMechanics& GridEntity::movement() const
{
    return *movement_;
}