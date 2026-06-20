#include "entities/player/Player.h"

Player::Player(std::unique_ptr<IMovementMechanics> movement)
    : Entity(std::move(movement))
{
}