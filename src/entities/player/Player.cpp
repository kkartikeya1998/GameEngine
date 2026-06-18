#include "entities/player/Player.h"

Player::Player(std::unique_ptr<IMovementMechanics> movement)
    : movement_(std::move(movement)) {}

void Player::move(Direction dir) {
    movement_->move(dir);
}

int Player::getX() const {
    return movement_->getPosition().x;
}

int Player::getY() const {
    return movement_->getPosition().y;
}

Direction Player::getDirection() const {
    return movement_->getDirection();
}

IMovementMechanics& Player::movement() {
    return *movement_;
}