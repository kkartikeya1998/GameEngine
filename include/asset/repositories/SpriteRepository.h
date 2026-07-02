#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "system/GameConstants.h"
#include "asset/repositories/IAssetRepository.h"

// ---------------------------------------------------------------------------
// SpriteFrameMetadata — where one named animation frame lives on the
// player's spritesheet.
//
// Keyed by frame name (e.g. "player_down_0") in SpriteRepository — mirrors
// how TileTypeMetadata is keyed by tile type name in MapRepository.
// ---------------------------------------------------------------------------
struct SpriteFrameMetadata {
    std::string texturePath;
    sf::IntRect textureRect;
    std::string spriteName;
    int sourceTileSize = GameConstants::TILE_SIZE;
};

// ---------------------------------------------------------------------------
// SpriteRepository — owns all player animation frame metadata, loaded once
// from a sprite metadata JSON file. Derives from IAssetRepository so
// AssetManager can own and serve it alongside the other metadata
// repositories.
//
// SpriteAtlas looks up each (direction, walk frame) combination's rect here
// by name rather than hardcoding coordinates — see
// SpriteAtlas::getPlayerSprite.
// ---------------------------------------------------------------------------
class SpriteRepository : public IAssetRepository {
public:
    explicit SpriteRepository(const std::filesystem::path& metadataFilePath);

    // Returns nullptr if the frame name isn't in the catalog.
    const SpriteFrameMetadata* find(const std::string& frameName) const;

private:
    std::unordered_map<std::string, SpriteFrameMetadata> frames_;

    void load_from_file(const std::filesystem::path& path);
};