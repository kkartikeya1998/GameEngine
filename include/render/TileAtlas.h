#pragma once

#include "render/Atlas.h"
#include "world/Terrain.h"
#include "world/MapRepository.h"
#include <unordered_map>
#include <string>

// ---------------------------------------------------------------------------
// TileAtlas — terrain tile sprite lookup.
//
// Every texture rect comes fromMapRepository, which loads it from a tile 
// metadata JSON file. This class only converts a Terrain::Type into the 
// matching SpriteRegion.
// ---------------------------------------------------------------------------
class TileAtlas : public Atlas {
public:
    TileAtlas(const std::string& spritesheet_path, const MapRepository& mapRepository);

    const sf::Texture& terrainTexture() const { return texture(); }
    SpriteRegion getTerrainSprite(Terrain::Type terrain) const;

private:
    const MapRepository& mapRepository_; // non-owning, repo outlives atlas
    std::unordered_map<Terrain::Type, std::string> terrain_region_;
};