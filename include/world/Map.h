#pragma once

#include <vector>

#include "world/Tile.h"
#include "component/PositionComponent.h" // for AABB

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

    // Rejects the box if any tile it overlaps is TerrainType::Solid.
    // Caller is expected to have already bounds-checked the box (see
    // MovementSystem::mapBoundsCheck) — this does not clamp coordinates.
    bool isWalkable(const AABB& box) const;
};