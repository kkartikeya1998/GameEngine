// include/render/SpriteAtlas.h

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>

#include "world/Terrain.h"
#include "entities/movement/Position.h"

struct SpriteRegion {
    sf::Rect<int> subrect;
    sf::Vector2f tile_size; // optional, for scaling
};

class SpriteAtlas {
public:
    explicit SpriteAtlas(const std::string& spritesheet_path);

    // Get texture and subrect for terrain
    const sf::Texture& terrainTexture() const;
    SpriteRegion getTerrainSprite(Terrain::Type terrain) const;

    // Get texture and subrect for player (optionally with direction)
    const sf::Texture& playerTexture() const;
    SpriteRegion getPlayerSprite(Direction facing) const;

private:
    void loadRegion(const std::string& name, int x, int y, int w, int h);

    std::unique_ptr<sf::Texture> texture_;
    std::unordered_map<std::string, SpriteRegion> regions_;

    // map terrain → region name
    std::unordered_map<Terrain::Type, std::string> terrain_region_;
    std::unordered_map<std::string, SpriteRegion> player_regions_;
};