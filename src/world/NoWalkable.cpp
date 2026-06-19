#pragma once
#include "world/IWalkable.h"

// ---------------------------------------------------------------------------
// StaticBlock — minimal IWalkable that always blocks.
//
// Used by Map when applying a MapObject's footprint: any cell marked
// `blocking: true` gets one of these attached so Tile::isWalkable()
// returns false regardless of the tile's terrain.
// ---------------------------------------------------------------------------
class StaticBlock : public IWalkable {
public:
    bool canWalk() const override { return false; }
};