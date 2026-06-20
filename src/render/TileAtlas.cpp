#include "render/TileAtlas.h"
#include <stdexcept>

TileAtlas::TileAtlas(const std::string& spritesheet_path, const TileRepository& tileRepository)
    : Atlas(spritesheet_path)
    , tileRepository_(tileRepository)
{
    loadRegion("grass", 0, 0, 16, 16);
    loadRegion("sand", 16, 0, 16, 16);
    loadRegion("water", 32, 0, 16, 16);
    loadRegion("ice", 48, 0, 16, 16);
    loadRegion("cave", 64, 0, 16, 16);
    loadRegion("building", 80, 0, 16, 16);
    loadRegion("tree", 96, 0, 16, 16);

    terrain_region_[Terrain::Type::Grass] = "grass";
    terrain_region_[Terrain::Type::Sand] = "sand";
    terrain_region_[Terrain::Type::Water] = "water";
    terrain_region_[Terrain::Type::Ice] = "ice";
    terrain_region_[Terrain::Type::Cave] = "cave";
    terrain_region_[Terrain::Type::Building] = "building";
    terrain_region_[Terrain::Type::Tree] = "tree";
}

SpriteRegion TileAtlas::getTerrainSprite(Terrain::Type terrain) const
{
    auto it = terrain_region_.find(terrain);
    if (it == terrain_region_.end()) {
        throw std::runtime_error("No sprite region for terrain: " + std::to_string(static_cast<int>(terrain)));
    }
    return findRegion(it->second);
}