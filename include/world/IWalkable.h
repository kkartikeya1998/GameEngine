#pragma once

// ---------------------------------------------------------------------------
// IWalkable — capability interface for tiles that can be walked on.
//
// Most tiles are walkable by default based on terrain (see Terrain.h).
// This interface exists for tiles whose walkability is NOT determined by
// terrain alone — e.g. a boulder sitting on grass, or a bridge over water.
// ---------------------------------------------------------------------------
class IWalkable {
public:
    virtual ~IWalkable() = default;
    virtual bool canWalk() const = 0;
};

// ---------------------------------------------------------------------------
// NoWalkable — minimal IWalkable that always blocks.
//
// Used by Map when applying a MapObject's footprint: any cell marked
// `blocking: true` gets one of these attached so Tile::isWalkable()
// returns false regardless of the tile's terrain.
// ---------------------------------------------------------------------------
class NoWalkable : public IWalkable {
public:
    bool canWalk() const override { return false; }
};