#pragma once

#include "render/atlases/Atlas.h"
#include "asset/repositories/TileRepository.h"
#include <string>

// ---------------------------------------------------------------------------
// TileAtlas — tile sprite lookup.
// The repository lookup, null-check, and SpriteRegion construction still
// live in Atlas::regionFromRepository — see Atlas.h.
// ---------------------------------------------------------------------------
class TileAtlas : public Atlas {
public:
    TileAtlas(const std::filesystem::path& spritesheet_path, const TileRepository& tileRepository);

    const sf::Texture& getTileTexture() const { return texture(); }
    SpriteRegion getTileSprite(const std::string& typeName) const;

private:
    const TileRepository& tileRepository_;
};