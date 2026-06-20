#pragma once

#include "render/Atlas.h"
#include "world/Terrain.h"
#include "world/TileRepository.h"
#include <unordered_map>
#include <string>

// ---------------------------------------------------------------------------
// TileAtlas — terrain tile sprite lookup.
//
// Owns the spritesheet for terrain tiles and maps each Terrain::Type to its
// SpriteRegion. No animation, no direction — terrain tiles are static.
// ---------------------------------------------------------------------------
class TileAtlas : public Atlas {
public:
    TileAtlas(const std::string& spritesheet_path, const TileRepository& tileRepository);

    const sf::Texture& terrainTexture() const { return texture(); }
    SpriteRegion getTerrainSprite(Terrain::Type terrain) const;

private:
    const TileRepository& tileRepository_; // non-owning, repo outlives atlas
    std::unordered_map<Terrain::Type, std::string> terrain_region_;
};