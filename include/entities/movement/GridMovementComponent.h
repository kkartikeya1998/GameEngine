#pragma once

#include "entities/Component.h"
#include "entities/movement/Position.h"

// ---------------------------------------------------------------------------
// GridMovementComponent — pure data for discrete, tile-snapped movement.
//
// CHANGED (PositionComponent pass): gridX, gridY, facing are GONE from
// this struct — they now live on a sibling PositionComponent (as
// floats — see PositionComponent.h's "GRID ENTITIES" note). This
// struct keeps only the edge-trigger gate and the moved-this-frame
// flag, neither of which is positional.
//
// MovementSystem::updateGrid/getGridHitbox now take a
// PositionComponent& alongside this component — see MovementSystem.h.
// Anywhere that read movement.gridX/gridY as an int (tile-index math
// in MovementSystem.cpp's nextGridPos/tileBoxAt) now reads
// position.x/y as float and casts explicitly at the point it's used
// as a tile index.
//
// Field-for-field, this is what survives from the previous version:
//   lastInputDir -> lastInputDir
//   isMoving     -> isMoving
// (gridX, gridY, facing moved out to PositionComponent.)
// ---------------------------------------------------------------------------
struct GridMovementComponent : public Component {
    // Edge-trigger gate — see MovementSystem::updateGrid. Cleared back to
    // NONE the frame inputDir is released, so the same direction can
    // re-trigger a hop later. Without this, a held key would cause a
    // hop on every single frame once update() became unconditional.
    Direction lastInputDir = Direction::NONE;

    // True only on the frame a hop actually committed (move accepted,
    // not blocked). False every other frame, including frames where
    // inputDir was held but already gated, or where the move was
    // refused by collision.
    bool isMoving = false;
};