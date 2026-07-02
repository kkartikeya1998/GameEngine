#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>
#include "system/GameConstants.h"
#include "asset/repositories/IAssetRepository.h"
#include "asset/AsssetPaths.h"

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
// CollisionBox — an object's precise collidable rectangle, authored in the
// SAME pixel space as the object's sprite on its source texture (the same
// space textureRect's w/h is measured in) — NOT tile-fractions. This means
// you can eyeball collision_box values directly against the sprite in an
// image editor: "the flame base is about 40px wide" maps straight to
// width: 40, no unit conversion needed at authoring time.
//
// offsetX/offsetY: displacement of the collision box's BOTTOM-CENTER from
// the object's own placement anchor (its origin on the map — see
// MapObject::originX_/originY_ and SFMLRenderer::drawMapObject, which both
// treat origin as the sprite's center-bottom "feet" point directly, not
// derived from w/h). offset 0,0 means the box's bottom edge sits exactly
// on that anchor, growing UPWARD — matching how an object visually rests
// on the ground, rather than straddling it. width/height: box size in
// native pixels, extending upward and horizontally-centered from that
// bottom-center point.
//
// At collision-query time, MapObject::getCollisionBox scales ALL FOUR
// fields by the object's own (TILE_SIZE / sourceTileSize) factor — the
// exact same scale drawMapObject applies to the sprite — so the hitbox
// grows and shrinks in lockstep with the rendered sprite automatically.
// No separate fraction to maintain per object, and no need to recompute
// anything if the source art is re-exported at a different resolution.
//
// A campfire whose visible flame base is roughly 40x20px (in native,
// unscaled pixels) and rests right at the feet would use offsetX = 0,
// offsetY = 0, width = 40, height = 20.
// ---------------------------------------------------------------------------
struct CollisionBox {
    float offsetX = 0.f;
    float offsetY = 0.f;
    float width = 0.f;
    float height = 0.f;
};

// name added alongside texturePath/textureRect/footprint, mirroring the
// same addition to TileTypeMetadata and SpriteFrameMetadata — lets an
// ObjectTypeMetadata be passed around standalone without losing track of
// which catalog entry it came from.
//
// sourceTileSize: pixels-per-tile in this entry's source art — NOT the
// same as textureRect's width/height for objects, since an object can
// span multiple tiles. Defaults to 32.
//
// the same space.
struct ObjectTypeMetadata {
    std::string name;
    std::string texturePath;
    sf::IntRect textureRect;
    std::vector<FootprintCell> footprint;
    int sourceTileSize = GameConstants::TILE_SIZE;
    std::optional<CollisionBox> collisionBox;
};


class MapObjectRepository : public IAssetRepository {
public:
    explicit MapObjectRepository(const std::filesystem::path& metadataFilePath);

    // Programmatic registration — distinct from file loading. Useful for
    // tests or hardcoded checks. Unaffected by the constructor
    // change above.
    void register_type(const std::string& typeName, ObjectTypeMetadata meta);

    const ObjectTypeMetadata* find(const std::string& type) const;

private:
    void load_from_file(const std::filesystem::path& path);
    std::unordered_map<std::string, ObjectTypeMetadata> types_;
};