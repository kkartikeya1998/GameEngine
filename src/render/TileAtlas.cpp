#include "render/TileAtlas.h"
#include <stdexcept>

TileAtlas::TileAtlas(const std::string& spritesheet_path, const TileRepository& tileRepository)
    : Atlas(spritesheet_path)
    , tileRepository_(tileRepository)
{
}

SpriteRegion TileAtlas::getTerrainSprite(const std::string& typeName) const
{
    return regionFromRepository(tileRepository_, typeName, typeName);
}