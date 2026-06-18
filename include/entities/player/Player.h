#pragma once

#include "utils/Direction.h"

// ---------------------------------------------------------------------------
// Player — minimal position and facing state.
//
// Stores only position (x, y) and facing direction. The current map is
// maintained by World, not stored here. MovementSystem queries World for
// the active map and updates Player position.
//
// Ownership:
//   Player owns nothing. It's a pure data holder.
// ---------------------------------------------------------------------------
class Player {
public:
    Player(int startX, int startY, Direction facing = Direction::DOWN);

    // Position accessors
    int getX() const { return x_; }
    int getY() const { return y_; }

    // Direction accessors
    Direction getDirection() const { return direction_; }
    void changeDirection(Direction newDir);

    // Position mutators — called by MovementSystem
    void moveTo(int x, int y);

    // Returns the next tile position if moving in current direction.
    // Does NOT move the player — just computes the target.
    // Caller (MovementSystem) checks walkability and calls moveTo() if valid.
    void getNextPosition(int& outX, int& outY) const;

private:
    int       x_;
    int       y_;
    Direction direction_;
};