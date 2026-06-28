#pragma once

#include "entities/movement/Position2D.h"

// ---------------------------------------------------------------------------
// GridMovementComponent — pure data for discrete, tile-snapped movement.
//
// REPLACES GridMovementMechanics as a class. All the logic that used to
// live in GridMovementMechanics::update() (edge-trigger gate, collision
// check against the destination tile, all-or-nothing position+facing
// commit) now lives in MovementSystem::updateGrid() instead — this
// struct only holds the state that logic reads and writes.
//
// Field-for-field, this is GridMovementMechanics' old private section:
//   position_ (grid-space int)   -> gridX, gridY
//   lastInputDir_ (edge gate)     -> lastInputDir
//   wasMoving_                   -> isMoving
// position_.dir is split out as `facing`, matching how Position2D bundles
// x/y/dir together but the component exposes them as plain fields since
// there's no behavior left to encapsulate them behind.
// ---------------------------------------------------------------------------
struct GridMovementComponent {
    int gridX = 0;
    int gridY = 0;
    Direction facing = Direction::NONE;

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