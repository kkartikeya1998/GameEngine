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
    SpriteRegion getTileSprite(const std::string& typeName) const;

private:
    const ComponentAssetRepository<RenderAssetMetadata>& renderRepository_;
};