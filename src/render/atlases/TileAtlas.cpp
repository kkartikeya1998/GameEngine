#include "render/atlases/TileAtlas.h"
#include <stdexcept>

TileAtlas::TileAtlas(const std::filesystem::path& spritesheet_path, const TileRepository& tileRepository)
    : Atlas(spritesheet_path)
    , tileRepository_(tileRepository)
{
}

SpriteRegion TileAtlas::getTileSprite(const std::string& typeName) const
{
    return regionFromRepository(tileRepository_, typeName, typeName);
}