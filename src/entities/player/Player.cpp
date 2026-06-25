#include "entities/player/Player.h"

Player::Player(std::unique_ptr<IFreeMovementMechanics> movement,
               float walkCyclesPerSecond)
    : FreeEntity(std::move(movement), walkCyclesPerSecond)
{
}