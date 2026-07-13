#pragma once
#include "entities/Component.h"
#include <unordered_set>
#include <cstddef>

// Grid tile coordinate — x,y in tile units (world_x / TILE_SIZE),
// matching how MapLoader::loadMapById already indexes tiles.
struct TileCoord {
    int x = 0;
    int y = 0;
    bool operator==(const TileCoord& o) const { return x == o.x && y == o.y; }
};

struct TileCoordHash {
    std::size_t operator()(const TileCoord& t) const {
        return std::hash<long long>()(
            (static_cast<long long>(t.x) << 32) ^ static_cast<unsigned int>(t.y));
    }
};

// Restricts an entity's valid tiles for e.g. wander targets.
// Empty set = unrestricted. CreatureAISystem checks isAllowed()
// before committing to a target; a disallowed tile is never chosen.
struct TileRestrictionComponent : public Component {
    std::unordered_set<TileCoord, TileCoordHash> allowedTiles;

    TileRestrictionComponent() = default;
    explicit TileRestrictionComponent(std::unordered_set<TileCoord, TileCoordHash> tiles)
        : allowedTiles(std::move(tiles)) {}

    bool isAllowed(int tileX, int tileY) const {
        if (allowedTiles.empty()) return true;
        return allowedTiles.count(TileCoord{tileX, tileY}) > 0;
    }
};