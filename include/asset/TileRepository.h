#pragma once

#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "asset/IAssetRepository.h"
#include "world/Terrain.h"

// ---------------------------------------------------------------------------
// TileTypeMetadata — full description of one tile TYPE.
//
// name/texturePath added alongside the existing textureRect/terrainType so
// that a TileTypeMetadata can be passed around standalone (e.g. handed to a
// renderer) without losing track of which catalog entry it came from or
// where its source image lives. Mirrors the same fields now present on
// SpriteFrameMetadata and ObjectTypeMetadata.
//
// terrainType is currently a raw string (was Terrain::Type) — conversion
// via Terrain::terrain_from_string is no longer done at load time; callers
// that need the enum should convert it themselves for now.
// ---------------------------------------------------------------------------
struct TileTypeMetadata {
    std::string name;
    std::string texturePath;
    sf::IntRect textureRect;
    std::string terrainType;
};

// ---------------------------------------------------------------------------
// TileRepository — owns all tile type metadata, loaded once from a JSON
// file at construction. Derives from IAssetRepository so AssetManager can
// own and serve it alongside the other metadata repositories.
//
// Renamed from MapRepository — "Map" was misleading since this class
// never touches a Map instance, only tile-type catalog data.
// ---------------------------------------------------------------------------
class TileRepository : public IAssetRepository {
public:
    explicit TileRepository(const std::string& metadataFilePath);

    const TileTypeMetadata* find(const std::string& type) const;

private:
    void load_from_file(const std::string& path);
    std::unordered_map<std::string, TileTypeMetadata> types_;
};