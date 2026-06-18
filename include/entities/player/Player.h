#pragma once

#include "entities/movement/IMovementMechanics.h"

// ---------------------------------------------------------------------------
// Player — minimal state holder for the player character.
//
// Ownership:
//   Player owns nothing. It's a pure data holder.
// ---------------------------------------------------------------------------
class Player {
public:
    Player(std::unique_ptr<IMovementMechanics> movement);

    void move(Direction dir);

    int getX() const;
    int getY() const;
    Direction getDirection() const;
    
    IMovementMechanics& movement();

private:
    std::unique_ptr<IMovementMechanics> movement_;
};