#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "asset/MapObjectRepository.h"

// ---------------------------------------------------------------------------
// MapObject — one placed instance of an object type on a Map.
//
// ObjectTypeMetadata and FootprintCell moved to MapObjectRepository.h —
// MapObjectRepository is their real owner/loader, MapObject only ever
// holds a non-owning pointer into it. See MapObjectRepository.h for the
// rationale.
//
// ADDED: getTypeName() — exposes metadata_->name. Needed so a renderer can
// take a MapObject and ask MapObjectAtlas::getObjectSprite(typeName) for
// its sprite region, the same way drawTile already goes from a Tile's
// Terrain::Type to TileAtlas::getTerrainSprite.
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
    int getOriginX() const;
    int getOriginY() const;
    int getTeleportTargetMapId() const;
    void getTeleportTarget(int& x, int& y) const;

private:
    const ObjectTypeMetadata* metadata_;
    int originX_;
    int originY_;
    int teleportTargetMapId_;
    int teleportTargetX_;
    int teleportTargetY_;
};