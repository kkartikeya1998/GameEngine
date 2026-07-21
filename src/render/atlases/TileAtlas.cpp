#include "render/atlases/TileAtlas.h"

TileAtlas::TileAtlas(const std::filesystem::path& spritesheet_path,
                      const ComponentAssetRepository<RenderAssetMetadata>& renderRepository)
    : Atlas(spritesheet_path)
    , renderRepository_(renderRepository)
{
    LOG_INFO("Initializing atlas with spritesheet: " + spritesheet_path.string());
}

Result<SpriteRegion, AssetError> TileAtlas::getTileSprite(const std::string& typeName) const
{
    LOG_INFO("Fetching tile: " + typeName);
    return regionFromRepository(renderRepository_, typeName, typeName);
}