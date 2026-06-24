#pragma once

#include "render/Atlas.h"
#include "world/Terrain.h"
#include "asset/TileRepository.h"
#include <unordered_map>
#include <string>

// ---------------------------------------------------------------------------
// TileAtlas — terrain tile sprite lookup.
//
// getTerrainSprite's only job now is building the lookup key (Terrain::Type
// -> type name string). The actual repository lookup, null-check, and
// SpriteRegion construction live in Atlas::regionFromRepository — see
// Atlas.h for why that moved there.
// ---------------------------------------------------------------------------
class TileAtlas : public Atlas {
public:
    TileAtlas(const std::string& spritesheet_path, const TileRepository& tileRepository);

    const sf::Texture& terrainTexture() const { return texture(); }
    SpriteRegion getTerrainSprite(Terrain::Type terrain) const;

private:
    const TileRepository& tileRepository_;
    std::unordered_map<Terrain::Type, std::string> terrain_region_;
};