#pragma once

#include <vector>

#include "world/Tile.h"

// ---------------------------------------------------------------------------
// Map — pure in-memory representation of a loaded map's static level data.
//
// Holds tiles only. No entities, no objects, no npcs — those live in
// Registry now. Knows nothing about loading from file; MapLoader's job.
// ---------------------------------------------------------------------------
class Map {
private:
    std::vector<Tile> tiles;

    int width{0};
    int height{0};

    std::size_t index(int x, int y) const;

public:
    Map() = delete;
    Map(int width, int height);

    int getHeight() const { return height; }
    int getWidth() const { return width; }

    Tile& tile_at(int x, int y);
    const Tile& tile_at(int x, int y) const;
    void set_tile(int x, int y, Tile tile);
};