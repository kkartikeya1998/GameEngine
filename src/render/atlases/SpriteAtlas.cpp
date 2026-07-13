#include "render/atlases/SpriteAtlas.h"
#include <stdexcept>
#include <iostream>

SpriteAtlas::SpriteAtlas(const std::filesystem::path& spritesheet_path,
                          const ComponentAssetRepository<RenderAssetMetadata>& renderRepository)
    : Atlas(spritesheet_path), renderRepository_(renderRepository) {}

SpriteRegion SpriteAtlas::getCharacterSprite(const std::string& spriteName) const
{
    return regionFromRepository(renderRepository_, spriteName, spriteName);
}