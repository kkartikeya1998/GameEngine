#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Tile.h"
#include "MapObject.h"
#include "entities/movement/Position2D.h"
// ---------------------------------------------------------------------------
// Map — pure in-memory representation of a loaded map.
//
// Holds tiles and placed objects. Knows nothing about laoding from
// file. Building a Map from a file is MapLoader's job.
//
// Construction:
//   Map is built empty via the constructor (just dimensions), then filled
//   in by MapLoader via set_tile() and addMapObject(). This makes Map
//   trivially testable without ever touching a file.
//
// ADDED: isAreaBlocked. Map has no concept of pixel sizes anywhere else
// (tile_at takes grid coordinates throughout) — tileSize is passed in by
// the caller (the renderer's TILE_SIZE constant) rather than Map owning
// or knowing about a rendering constant. This is the one query continuous
// (free) movement needs that grid movement never did: "does this
// world-space box overlap any blocking tile," rather than "is this one
// specific grid cell walkable."
// ---------------------------------------------------------------------------
class Map {
private:
    std::vector<Tile> tiles;
    std::vector<std::unique_ptr<MapObject>> map_objects;

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
    void set_tile(int x, int y, Terrain::Type terrain, std::string typeName);

    // Takes ownership of a fully-constructed MapObject (built by MapLoader).
    void addMapObject(std::unique_ptr<MapObject> obj);

    const std::vector<std::unique_ptr<MapObject>>& getMapObjects() const {
        return map_objects;
    }

    // True if box (world/pixel space) overlaps any non-walkable tile, or
    // falls outside the map's bounds. tileSize is the caller's render
    // tile size (e.g. SFMLRenderer::TILE_SIZE) — Map stays agnostic of
    // any specific rendering constant, the caller supplies it.
    bool isAreaBlocked(const AABB& box, float tileSize) const;

    // void render() const;
};