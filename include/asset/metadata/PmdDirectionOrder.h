#pragma once
#include "component/DirectionComponent.h"

// Maps engine Direction (4-way) onto PMD's 8-row sheet layout, using
// only the 4 cardinal rows. Diagonal rows (1,3,5,7) go unused until the
// engine supports diagonal movement.
namespace PmdDirectionOrder {
    inline int rowFor(Direction d) {
        switch (d) {
            case Direction::DOWN:  return 0;
            case Direction::RIGHT: return 2;
            case Direction::UP:    return 4;
            case Direction::LEFT:  return 6;
            default:                return 0; // NONE falls back to Down-facing
        }
    }
}