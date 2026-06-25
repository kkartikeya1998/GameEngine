#pragma once
#include <optional>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "asset/MapObjectRepository.h"
#include "entities/movement/Position2D.h"

// ---------------------------------------------------------------------------
// MapObject — one placed instance of an object type on a Map.
//
// ObjectTypeMetadata and FootprintCell reside in MapObjectRepository.h —
// MapObjectRepository is their real owner/loader, MapObject only ever
// holds a non-owning pointer into it.
// ---------------------------------------------------------------------------
class MapObject {
public:
    MapObject(
        const ObjectTypeMetadata* metadata,
        int originX,
        int originY,
        int teleportTargetMapId = -1,
        int teleportTargetX = 0,
        int teleportTargetY = 0
    );

    const std::string& getTypeName() const;
    const std::string& getTexturePath() const;
    const std::vector<FootprintCell>& getFootprint() const;
    int getOriginPixelX() const;
    int getOriginPixelY() const;
    int getTeleportTargetMapId() const;
    void getTeleportTarget(int& x, int& y) const;

    // Resolves this object's collisionBox (if it has one) into a
    // world-space AABB, using tileSize to convert the metadata's
    // tile-fraction units into pixels. Returns nullopt if metadata_ has
    // no collisionBox set.
    std::optional<AABB> getCollisionBox(float tileSize) const;

private:
    const ObjectTypeMetadata* metadata_;
    int originX_;
    int originY_;
    int teleportTargetMapId_;
    int teleportTargetX_;
    int teleportTargetY_;
};