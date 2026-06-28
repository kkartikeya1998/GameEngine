#pragma once

#include <memory>
#include <vector>

#include "world/Map.h"
#include "world/MapLoader.h"
#include "asset/MapObjectRepository.h"
#include "entities/Entity.h"

// ---------------------------------------------------------------------------
// World — owns the active Map and the MapLoader that builds it.
//
// CHANGED (Component base pass): World gains a registry-style query
// front door over whatever the active Map currently holds. Storage
// itself stays exactly where it already was — Map::map_objects/npcs_ —
// this is NOT a move of ownership into World (that was the
// alternative considered and explicitly NOT chosen, since maps swap
// out via loadMap() and per-Map ownership matches that lifetime
// already). World::getAllEntities()/view<T>() just give callers (like
// RenderSystem) one place to ask "every entity" or "every entity with
// component T" instead of reaching past World into Map's two separate
// getMapObjects()/getNpcs() accessors directly.
//
// Per the linear-curve precedent: this does NOT include the player.
// player_ lives on GameController, not on Map/World, and there has
// been no second concrete caller yet that needs player+NPC+map-object
// treated uniformly — RenderSystem still has its own explicit player
// handling. If/when that pressure shows up, extending these queries to
// include the player is a small additive change, not a redesign.
//
// view<T>() is O(total entities) per call, same dynamic_cast-based
// has<T>() Entity already exposes — no caching, no index. Fine for the
// current entity counts (a handful of NPCs/objects per map); revisit
// only if profiling says otherwise.
// ---------------------------------------------------------------------------
class World {
public:
    World(const std::string& mapsFolder, MapObjectRepository& objectRepository);

    Map* getActiveMap() const;

    void loadMap(int mapId);

    // Every entity on the active map — map objects and NPCs combined,
    // in that order. Empty if no map is loaded. Does NOT include the
    // player — see class comment above.
    std::vector<Entity*> getAllEntities() const;

    // Every entity on the active map that currently has a component of
    // type T attached, e.g. world.view<GridMovementComponent>() to find
    // every grid-moving entity regardless of whether it's an NPC or a
    // (currently hypothetical) grid-moving map object.
    template <typename T>
    std::vector<Entity*> view() const
    {
        std::vector<Entity*> result;
        for (Entity* e : getAllEntities()) {
            if (e->has<T>())
                result.push_back(e);
        }
        return result;
    }

private:
    MapLoader loader_;

    std::unique_ptr<Map> active_map_;
};