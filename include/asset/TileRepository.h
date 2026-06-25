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
//
// sourceTileSize. Pixels-per-tile in this entry's own source art.
// For a tile this is normally equal to textureRect's width/height (a tile
// occupies exactly one grid cell by definition) — having it as explicit
// data rather than inferred from w/h means re-exporting the tileset at a
// different source resolution (e.g. 32px -> 128px) is a JSON change, not
// a renderer code change. The render system's own TILE_SIZE constant
// (the on-screen pixel size of one tile) stays completely independent of
// this value; the scale factor connecting them is TILE_SIZE /
// sourceTileSize, computed per-lookup in TileAtlas, never hardcoded.
//
// Defaults to 32 to match the project's existing JSON without requiring
// every tileset_metadata.json entry to be updated immediately.
// ---------------------------------------------------------------------------
struct TileTypeMetadata {
    std::string name;
    std::string texturePath;
    sf::IntRect textureRect;
    std::string terrainType;
    int sourceTileSize = 32;
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