#include "render/TileAtlas.h"
#include <stdexcept>

TileAtlas::TileAtlas(const std::string& spritesheet_path, const MapRepository& mapRepository)
    : Atlas(spritesheet_path)
    , mapRepository_(mapRepository)
{
    // Bridge enum -> type name only. No coordinates here — those live in
    // the tile metadata JSON and are read through mapRepository_.
    terrain_region_[Terrain::Type::Grass]    = "Grass";
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

    const TileTypeMetadata* meta = mapRepository_.find(it->second);
    if (!meta) {
        throw std::runtime_error("MapRepository has no metadata for tile type: " + it->second);
    }

    SpriteRegion region;
    region.subrect = meta->textureRect;
    region.tile_size = sf::Vector2f(
        static_cast<float>(meta->textureRect.size.x),
        static_cast<float>(meta->textureRect.size.y)
    );
    return region;
}