#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "asset/IAssetRepository.h"

// ---------------------------------------------------------------------------
// FootprintCell / ObjectTypeMetadata — moved here from MapObject.h.
//
// MapObjectRepository is the actual owner and loader of this metadata
// (MapObject only ever holds a non-owning const pointer into this
// repository's types_ map), so the struct now lives where it's defined and
// loaded, matching TileTypeMetadata living in TileRepository.h and
// SpriteFrameMetadata living in SpriteRepository.h. MapObject.h now
// includes this header to use the type rather than declaring it itself.
// ---------------------------------------------------------------------------
struct FootprintCell {
    int dx, dy;
    bool blocking;
    bool isTeleportTile;
};

// ---------------------------------------------------------------------------
// CollisionBox — an object's precise collidable rectangle, expressed as a
// fraction of one tile rather than pixels, so it stays resolution-
// independent (same reasoning as sourceTileSize) and is easy to author by
// hand in JSON without doing pixel math against a specific tile_size.
//
// offsetX/offsetY: top-left corner of the box, relative to the object's
// origin tile (0.0 = origin tile's left/top edge). width/height: box size,
// in tile units (1.0 = exactly one tile).
//
// A campfire that should only block its center half, for example, would
// use offsetX/offsetY = 0.25, width/height = 0.5 — a centered half-tile
// box — entirely in data, no code change required per object.
// ---------------------------------------------------------------------------
struct CollisionBox {
    float offsetX = 0.f;
    float offsetY = 0.f;
    float width = 1.f;
    float height = 1.f;
};

// name added alongside texturePath/textureRect/footprint, mirroring the
// same addition to TileTypeMetadata and SpriteFrameMetadata — lets an
// ObjectTypeMetadata be passed around standalone without losing track of
// which catalog entry it came from.
//
// sourceTileSize: pixels-per-tile in this entry's source art — NOT the
// same as textureRect's width/height for objects, since an object can
// span multiple tiles. Defaults to 32 to match existing JSON.
//
// ADDED: collisionBox. std::nullopt (the default) means "fall back to
// footprint-based whole-tile blocking" — every existing object without
// this field behaves exactly as before. When present, it REPLACES
// footprint-based blocking for that object (see MapLoader::applyFootprint
// for how that's enforced) — an object opting into a precise box gets
// exactly that box, not the box AND a whole blocked tile fighting over
// the same space.
struct ObjectTypeMetadata {
    std::string name;
    std::string texturePath;
    sf::IntRect textureRect;
    std::vector<FootprintCell> footprint;
    int sourceTileSize = 32;
    std::optional<CollisionBox> collisionBox;
};


class MapObjectRepository : public IAssetRepository {
public:
    explicit MapObjectRepository(const std::string& metadataFilePath);

    // Programmatic registration — distinct from file loading. Useful for
    // tests or hardcoded checks. Unaffected by the constructor
    // change above.
    void register_type(const std::string& typeName, ObjectTypeMetadata meta);

    const ObjectTypeMetadata* find(const std::string& type) const;

private:
    void load_from_file(const std::string& path);
    std::unordered_map<std::string, ObjectTypeMetadata> types_;
};