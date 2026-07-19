#include "render/atlases/TileAtlas.h"

TileAtlas::TileAtlas(const std::filesystem::path& spritesheet_path,
                      const ComponentAssetRepository<RenderAssetMetadata>& renderRepository)
    : Atlas(spritesheet_path)
    , renderRepository_(renderRepository)
{
}

Result<SpriteRegion, AssetError> TileAtlas::getTileSprite(const std::string& typeName) const
{
    return regionFromRepository(renderRepository_, typeName, typeName);
}