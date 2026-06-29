#pragma once

#include "entities/movement/Position.h"

// ---------------------------------------------------------------------------
// Position2D — continuous (float) counterpart to Position.
//
// Position2D is for entities whose true position is NOT snapped to a
// tile — i.e. anything using IFreeMovementMechanics. dir is kept as a
// field here too, mirroring Position, since facing direction is the same
// concept regardless of how position itself is represented.
// ---------------------------------------------------------------------------
struct Position2D {
    float x;
    float y;
    Direction dir = Direction::NONE;
};

// ---------------------------------------------------------------------------
// AABB — axis-aligned bounding box in world (pixel) space. Used for
// collision queries: an entity's hitbox, or a blocking tile's rectangle.
// ---------------------------------------------------------------------------
