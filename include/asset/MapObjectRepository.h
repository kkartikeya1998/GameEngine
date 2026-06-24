#pragma once

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

// name added alongside texturePath/textureRect/footprint, mirroring the
// same addition to TileTypeMetadata and SpriteFrameMetadata — lets an
// ObjectTypeMetadata be passed around standalone without losing track of
// which catalog entry it came from.
//
// ADDED: sourceTileSize. Pixels-per-tile in this entry's source art —
// NOT the same as textureRect's width/height for objects, since an object
// can span multiple tiles (House is 96x32px at sourceTileSize 32, meaning
// 3 tiles wide x 1 tile tall). This is what lets the renderer compute an
// object's true size in tiles (textureRect.size / sourceTileSize, per
// axis) without any hardcoded pixels-per-tile assumption baked into
// SFMLRenderer — see drawMapObject.
//
// Defaults to 32 to match the project's existing JSON without requiring
// every object_metadata.json entry to be updated immediately.
struct ObjectTypeMetadata {
    std::string name;
    std::string texturePath;
    sf::IntRect textureRect;
    std::vector<FootprintCell> footprint;
    int sourceTileSize = 32;
};

// ---------------------------------------------------------------------------
// MapObjectRepository — owns all object type metadata.
//
// CHANGED: previously default-constructed with load_from_file() called
// separately (by MapLoader, as a side effect of building a map — a
// responsibility leak, since MapLoader has no business deciding when
// object metadata loads). Now matches MapRepository and SpriteRepository's
// contract: the constructor takes the metadata path and loads immediately,
// so there's exactly one lifecycle for every repository AssetManager owns,
// and loading happens once, at AssetManager construction, not on first use
// from an arbitrary call site.
//
// load_from_file is now a private implementation detail of the
// constructor, not a separately-callable public step.
// ---------------------------------------------------------------------------
class MapObjectRepository : public IAssetRepository {
public:
    explicit MapObjectRepository(const std::string& metadataFilePath);

    // Programmatic registration — distinct from file loading. Useful for
    // tests or hardcoded fallback types. Unaffected by the constructor
    // change above.
    void register_type(const std::string& typeName, ObjectTypeMetadata meta);

    const ObjectTypeMetadata* find(const std::string& type) const;

private:
    void load_from_file(const std::string& path);
    std::unordered_map<std::string, ObjectTypeMetadata> types_;
};