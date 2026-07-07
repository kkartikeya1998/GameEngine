#pragma once

#include <memory>
#include <string>
#include <vector>

#include "world/Tile.h"
#include "entities/Entity.h"
// for AABB
#include "tmp/component/PositionComponent.h"
#include "tmp/component/RenderComponent.h"
#include "tmp/component/CollisionComponent.h"
#include "system/GameConstants.h"

// ---------------------------------------------------------------------------
// Map — pure in-memory representation of a loaded map.
//
// Holds tiles and placed objects. Knows nothing about loading from
// file. Building a Map from a file is MapLoader's job.
//
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
    void set_tile(int x, int y, Tile tile);

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
    // tile size (e.g. SFMLRenderer::TILE_SIZE).
    bool isAreaBlocked(const AABB& box) const;

};