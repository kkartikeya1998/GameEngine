#include "entities/player/Player.h"

Player::Player(int startX, int startY, Direction facing)
    : x_(startX)
    , y_(startY)
    , direction_(facing)
{}

void Player::changeDirection(Direction newDir) {
    direction_ = newDir;
}

void Player::moveTo(int x, int y) {
    x_ = x;
    y_ = y;
}

void Player::getNextPosition(int& outX, int& outY) const {
    outX = x_;
    outY = y_;

    switch (direction_) {
        case Direction::UP:    --outY; break;
        case Direction::DOWN:  ++outY; break;
        case Direction::LEFT:  --outX; break;
        case Direction::RIGHT: ++outX; break;
        case Direction::NONE:  break;
    }
}