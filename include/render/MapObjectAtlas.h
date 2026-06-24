#pragma once

#include "render/Atlas.h"
#include "asset/MapObjectRepository.h"
#include <string>
#include <unordered_map>

// ---------------------------------------------------------------------------
// MapObjectAtlas — sprite lookup for placed map objects (trees, houses, ...).
//
// CHANGED: object types are no longer assumed to share one spritesheet.
// Each ObjectTypeMetadata carries its own texturePath, which can be the
// same file as another type (e.g. several objects packed into one sheet)
// or a completely separate image (e.g. tree.png, house.png). getObjectSprite
// resolves and returns a SpriteRegion with its texture field set to
// whichever texture that type's metadata points at.
//
// Atlas::regionFromRepository isn't used here — that helper assumes a
// single shared texture, which no longer holds for this atlas. The lookup
// (find by type name, null-check, build SpriteRegion) is inlined in
// getObjectSprite instead, with texture resolution added on top.
//
// Atlas's own constructor still runs with spritesheet_path as a fallback
// texture (objectTexture()) — kept for any caller that wants "the" object
// texture without going through a specific type lookup. Most real lookups
// go through getObjectSprite and get a per-type texture instead.
//
// Textures are cached by path, so placing 50 trees on a map loads
// tree.png's texture exactly once, not once per placed instance.
// ---------------------------------------------------------------------------
class MapObjectAtlas : public Atlas {
public:
    MapObjectAtlas(const std::string& spritesheet_path, const MapObjectRepository& objectRepository);

    const sf::Texture& objectTexture() const { return texture(); }
    SpriteRegion getObjectSprite(const std::string& typeName) const;

private:
    const MapObjectRepository& objectRepository_;

    // Keyed by texture path. mutable: getObjectSprite is logically const
    // (it's a lookup), but populates the cache lazily on first use of
    // each distinct texture path.
    mutable std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textureCache_;

    const sf::Texture& getOrLoadTexture(const std::string& path) const;
};