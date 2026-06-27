#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Tile.h"
#include "MapObject.h"
#include "entities/movement/Position2D.h"
#include "entities/npc/Npc.h" // NEW
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
//
// NEW: npcs_ — a second owned collection, sibling to map_objects, NOT
// merged into it. MapObject has no update()/movement mechanic/position
// concept beyond a fixed origin; Npc does. Keeping them as two distinct
// collections on the same Map reflects that these are two genuinely
// different kinds of things that happen to both live spatially on a
// map, rather than forcing one container to serve two contracts.
// ---------------------------------------------------------------------------
class Map {
private:
    std::vector<Tile> tiles;
    std::vector<std::unique_ptr<MapObject>> map_objects;
    std::vector<std::unique_ptr<Npc>> npcs_; // NEW

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

    // NEW — same ownership pattern as addMapObject/getMapObjects above.
    // A map with no "npcs" entry in its JSON simply never calls this;
    // npcs_ stays empty, identical to today's behavior.
    void addNpc(std::unique_ptr<Npc> npc) {
        npcs_.push_back(std::move(npc));
    }

    const std::vector<std::unique_ptr<Npc>>& getNpcs() const {
        return npcs_;
    }

    // True if box (world/pixel space) overlaps any non-walkable tile, or
    // falls outside the map's bounds. tileSize is the caller's render
    // tile size (e.g. SFMLRenderer::TILE_SIZE) — Map stays agnostic of
    // any specific rendering constant, the caller supplies it.
    bool isAreaBlocked(const AABB& box, float tileSize) const;

    // void render() const;
};