// src/render/SpriteAtlas.cpp

#include "render/SpriteAtlas.h"
#include <iostream>
#include <sstream>

SpriteAtlas::SpriteAtlas(const std::string& spritesheet_path)
{
    texture_ = std::make_unique<sf::Texture>();
    if (!texture_->loadFromFile(spritesheet_path)) {
        throw std::runtime_error("Failed to load spritesheet: " + spritesheet_path);
    }

    // Example: define regions manually
    // You can later replace this with a JSON config.

    // Terrain regions (example coordinates, adjust to your spritesheet)
    loadRegion("grass", 0, 0, 16, 16);
    loadRegion("sand", 16, 0, 16, 16);
    loadRegion("water", 32, 0, 16, 16);
    loadRegion("ice", 48, 0, 16, 16);
    loadRegion("cave", 64, 0, 16, 16);
    loadRegion("building", 80, 0, 16, 16);
    loadRegion("tree", 96, 0, 16, 16);

    // Map terrain types to region names
    terrain_region_[Terrain::Type::Grass]   = "grass";
    terrain_region_[Terrain::Type::Sand]    = "sand";
    terrain_region_[Terrain::Type::Water]   = "water";
    terrain_region_[Terrain::Type::Ice]     = "ice";
    terrain_region_[Terrain::Type::Cave]    = "cave";
    terrain_region_[Terrain::Type::Building] = "building";
    terrain_region_[Terrain::Type::Tree]    = "tree";

    // Player regions (example: 4 directions in one row)
    loadRegion("player_up",    0, 16, 16, 16);
    loadRegion("player_down",  16, 16, 16, 16);
    loadRegion("player_left",  32, 16, 16, 16);
    loadRegion("player_right", 48, 16, 16, 16);

    player_regions_["UP"]    = regions_["player_up"];
    player_regions_["DOWN"]  = regions_["player_down"];
    player_regions_["LEFT"]  = regions_["player_left"];
    player_regions_["RIGHT"] = regions_["player_right"];
}

void SpriteAtlas::loadRegion(const std::string& name, int x, int y, int w, int h)
{
    SpriteRegion region;
    region.subrect = sf::Rect<int>(
        sf::Vector2<int>(x, y),
        sf::Vector2<int>(w, h)
    );
    region.tile_size = sf::Vector2f(w, h);
    regions_[name] = region;
}
const sf::Texture& SpriteAtlas::terrainTexture() const
{
    return *texture_;
}

SpriteRegion SpriteAtlas::getTerrainSprite(Terrain::Type terrain) const
{
    auto it = terrain_region_.find(terrain);
    if (it == terrain_region_.end()) {
        throw std::runtime_error("No sprite region for terrain: " + std::to_string(static_cast<int>(terrain)));
    }
    auto regionIt = regions_.find(it->second);
    if (regionIt == regions_.end()) {
        throw std::runtime_error("No region defined for: " + it->second);
    }
    return regionIt->second;
}

const sf::Texture& SpriteAtlas::playerTexture() const
{
    return *texture_;
}

SpriteRegion SpriteAtlas::getPlayerSprite(Direction facing) const
{
    std::string key;
    switch (facing) {
        case Direction::UP:    key = "UP"; break;
        case Direction::DOWN:  key = "DOWN"; break;
        case Direction::LEFT:  key = "LEFT"; break;
        case Direction::RIGHT: key = "RIGHT"; break;
        default:
            key = "DOWN";
    }
    auto it = player_regions_.find(key);
    if (it == player_regions_.end()) {
        throw std::runtime_error("No sprite region for player facing: " + key);
    }
    return it->second;
}