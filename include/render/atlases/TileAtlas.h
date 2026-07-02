#pragma once

#include "render/atlases/Atlas.h"
#include "asset/repositories/TileRepository.h"
#include <string>

// ---------------------------------------------------------------------------
// TileAtlas — tile sprite lookup.
//
// CHANGED: getTerrainSprite now takes the tile's catalog key (type name
// string, e.g. "Grass2") directly, instead of a Terrain::Type that then
// had to be translated back into a string via a hardcoded
// Terrain::Type -> name map. That bridge (terrain_region_) is gone — the
// renderer no longer needs to know about Terrain::Type at all, since
// Tile now carries its own catalog name (see Tile.h). This also means
// two tiles can share a Terrain::Type (same walkability rules) while
// rendering completely different art, which the old enum-keyed map
// couldn't express.
//
// The repository lookup, null-check, and SpriteRegion construction still
// live in Atlas::regionFromRepository — see Atlas.h.
// ---------------------------------------------------------------------------
class TileAtlas : public Atlas {
public:
    TileAtlas(const std::filesystem::path& spritesheet_path, const TileRepository& tileRepository);

    const sf::Texture& terrainTexture() const { return texture(); }
    SpriteRegion getTerrainSprite(const std::string& typeName) const;

private:
    const TileRepository& tileRepository_;
};