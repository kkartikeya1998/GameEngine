#include "render/TileAtlas.h"
#include <stdexcept>

TileAtlas::TileAtlas(const std::string& spritesheet_path, const TileRepository& tileRepository)
    : Atlas(spritesheet_path)
    , tileRepository_(tileRepository)
{
    terrain_region_[Terrain::Type::Grass1]    = "Grass1";
    terrain_region_[Terrain::Type::Grass2]    = "Grass2";
    terrain_region_[Terrain::Type::Grass3]    = "Grass3";
    terrain_region_[Terrain::Type::Grass4]    = "Grass4";
    terrain_region_[Terrain::Type::Sand]     = "Sand";
    terrain_region_[Terrain::Type::Water]    = "Water";
    terrain_region_[Terrain::Type::Ice]      = "Ice";
    terrain_region_[Terrain::Type::Cave]     = "Cave";
    terrain_region_[Terrain::Type::Building] = "Building";
    terrain_region_[Terrain::Type::Tree]     = "Tree";
}

SpriteRegion TileAtlas::getTerrainSprite(Terrain::Type terrain) const
{
    auto it = terrain_region_.find(terrain);
    if (it == terrain_region_.end()) {
        throw std::runtime_error("No tile type name mapped for terrain: " +
                                  std::to_string(static_cast<int>(terrain)));
    }

    return regionFromRepository(tileRepository_, it->second, it->second);
}