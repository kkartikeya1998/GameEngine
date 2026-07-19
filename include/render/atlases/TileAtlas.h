#pragma once

#include "render/atlases/Atlas.h"
#include "asset/repositories/ComponentAssetRepository.h"
#include "asset/metadata/RenderAssetMetadata.h"
#include <string>

class TileAtlas : public Atlas {
public:
    TileAtlas(const std::filesystem::path& spritesheet_path,
              const ComponentAssetRepository<RenderAssetMetadata>& renderRepository);

    const sf::Texture& getTileTexture() const { return texture(); }

    // Recoverable: an unresolvable typeName is content data (a bad map
    // tile id), not an engine invariant, and this is called once per
    // visible tile every frame — see RenderSystem::submitTile for how a
    // miss is handled (skip drawing that tile, log once).
    Result<SpriteRegion, AssetError> getTileSprite(const std::string& typeName) const;

private:
    const ComponentAssetRepository<RenderAssetMetadata>& renderRepository_;
};