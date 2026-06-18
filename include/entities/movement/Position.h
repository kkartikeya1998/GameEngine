#pragma once

// ---------------------------------------------------------------------------
// Direction and position 
//
// Used by:
//   - Player (facing direction)
//   - Wild/Captured Pokemon sprites (facing when rendered on tile)
//   - NPCs and other map entities
//
// This is a fundamental game concept, shared across all directional entities.
// ---------------------------------------------------------------------------
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};


struct Position {
    int x;
    int y;
    Direction dir = Direction::NONE;
};
