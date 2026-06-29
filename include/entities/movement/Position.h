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
// 
// [Update] Added Position2D.h: Position (int x, y) stays exactly as-is and keeps meaning "which grid
// cell" — Map, MapLoader, footprints, and GridMovementMechanics all
// legitimately want integer tile coordinates and aren't touched by this.
//
// ---------------------------------------------------------------------------
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

struct AABB {
    float x, y; 
    float width, height;

    bool intersects(const AABB& other) const {
        return x < other.x + other.width  && x + width  > other.x &&
               y < other.y + other.height && y + height > other.y;
    }
};