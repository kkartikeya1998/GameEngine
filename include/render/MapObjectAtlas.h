#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>

#include "world/MapObjectRepository.h"

// ---------------------------------------------------------------------------
// MapObjectAtlas — texture lookup for placed map objects (trees, houses, ...).
//
// Unlike TileAtlas (one shared spritesheet, many sub-rects) or SpriteAtlas
// (one shared spritesheet, many animation frames), each MapObject TYPE has
// its own full-image texture — ObjectTypeMetadata::texturePath already
// carries this per type. So this atlas owns a small cache of textures,
// lazily loaded on first use, keyed by type name.
//
// Still follows the same external contract as TileAtlas/SpriteAtlas: ask
// it for a drawable thing given a domain type, and it asks its paired
// repository for the metadata.
// ---------------------------------------------------------------------------
class MapObjectAtlas {
public:
    explicit MapObjectAtlas(const MapObjectRepository& objectRepository);

    // Returns the full texture for this object type. Throws if the type
    // isn't in the repository, or if its texture file fails to load.
    const sf::Texture& getObjectTexture(const std::string& typeName);

private:
    const MapObjectRepository& objectRepository_; // non-owning, repo outlives atlas

    // Lazily-loaded texture cache, keyed by type name. Loaded on first
    // request rather than eagerly, since the full set of placed object
    // types on a given map is usually a small subset of the catalog.
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures_;
};