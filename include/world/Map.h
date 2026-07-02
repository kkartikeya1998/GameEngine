#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Tile.h"
#include "Terrain.h"
#include "entities/Entity.h"
// #include "tmp/movement/Position2D.h"
#include "render/FreeRenderComponent.h"
#include "system/MapObjectSystem.h"
#include "system/GameConstants.h"

// ---------------------------------------------------------------------------
// Map — pure in-memory representation of a loaded map.
//
// Holds tiles and placed objects. Knows nothing about loading from
// file. Building a Map from a file is MapLoader's job.
//
// CHANGED (ECS pass): map_objects and npcs_ both become
// std::vector<std::unique_ptr<Entity>>, collapsed from two separately-
// typed collections (vector<unique_ptr<MapObject>>,
// vector<unique_ptr<Npc>>) into one shape. MapObject and Npc are no
// longer classes — see entities/MapObjectRenderComponent.h and
// entities/npc/NpcComponent.h. What used to distinguish "this is a
// MapObject" from "this is an Npc" by TYPE is now distinguished by
// WHICH COMPONENTS the Entity carries (MapObjectRenderComponent vs.
// NpcComponent + a movement component) — same data, same two
// conceptually-different kinds of things, just no longer needing two
// different container element types to express that difference.
//
// addMapObject/getMapObjects and addNpc/getNpcs are KEPT AS TWO
// SEPARATE METHOD PAIRS (not collapsed into one addEntity/getEntities)
// deliberately — MapLoader's call sites already know which kind of
// thing they're building, and collapsing the two would just mean every
// caller needs to re-derive "is this a map object or an npc" from
// component presence instead of from which method it already called.
// No second use case has shown up yet that wants a single uniform
// "all entities on this map" query, so per the linear-curve precedent
// this stays as two pairs until one does.
//
// isAreaBlocked is UNCHANGED — pixel-space collision query, no
// MapObject/Npc-shape dependency.
// ---------------------------------------------------------------------------
class Map {
private:
    std::vector<Tile> tiles;
    std::vector<std::unique_ptr<Entity>> map_objects;
    std::vector<std::unique_ptr<Entity>> npcs_;

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

    // Takes ownership of a fully-constructed MapObject Entity (built by
    // MapLoader — see MapLoader::buildMapObject, which attaches
    // MapObjectRenderComponent and optionally ColliderComponent/
    // TeleportComponent).
    void addMapObject(std::unique_ptr<Entity> obj) {
        map_objects.push_back(std::move(obj));
    }

    const std::vector<std::unique_ptr<Entity>>& getMapObjects() const {
        return map_objects;
    }

    // Takes ownership of a fully-constructed NPC Entity (built by
    // MapLoader::buildNpc, which attaches a movement component +
    // a render component + NpcComponent).
    void addNpc(std::unique_ptr<Entity> npc) {
        npcs_.push_back(std::move(npc));
    }

    const std::vector<std::unique_ptr<Entity>>& getNpcs() const {
        return npcs_;
    }

    // True if box (world/pixel space) overlaps any non-walkable tile, or
    // falls outside the map's bounds. tileSize is the caller's render
    // tile size (e.g. SFMLRenderer::TILE_SIZE) — Map stays agnostic of
    // any specific rendering constant, the caller supplies it.
    bool isAreaBlocked(const AABB& box) const;

};