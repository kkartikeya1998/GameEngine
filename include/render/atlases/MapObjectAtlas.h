#pragma once

#include "render/atlases/Atlas.h"
#include "asset/repositories/ComponentAssetRepository.h"
#include "asset/metadata/RenderAssetMetadata.h"
#include <string>
#include <unordered_map>

class MapObjectAtlas : public Atlas {
public:
    MapObjectAtlas(const std::filesystem::path& spritesheet_path,
                   const ComponentAssetRepository<RenderAssetMetadata>& renderRepository);
    const sf::Texture& objectTexture() const { return texture(); }
    SpriteRegion getObjectSprite(const std::string& typeName) const;
    const sf::Texture& resolveTexture(const std::string& texturePath) const { return getOrLoadTexture(texturePath); }
private:
    const ComponentAssetRepository<RenderAssetMetadata>& renderRepository_;
    mutable std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textureCache_;
    const sf::Texture& getOrLoadTexture(const std::string& path) const;
};