#include "world/Map.h"

#include <iostream>
#include <stdexcept>
#include <utility>

Map::Map(int width, int height)
    : width(width)
    , height(height)
{
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Map dimensions must be positive");
    }
    tiles.resize(static_cast<std::size_t>(width * height));
}

std::size_t Map::index(int x, int y) const {
    return static_cast<std::size_t>(y * width + x);
}

Tile& Map::tile_at(int x, int y) {
    return tiles[index(x, y)];
}

const Tile& Map::tile_at(int x, int y) const {
    return tiles[index(x, y)];
}

void Map::set_tile(int x, int y, Terrain::Type terrain) {
    tiles[index(x, y)] = Tile(terrain);
}

void Map::addMapObject(std::unique_ptr<MapObject> obj) {
    map_objects.push_back(std::move(obj));
}

// void Map::render() const {
//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             std::cout << tile_at(x, y).render_char();
//         }
//         std::cout << '\n';
//     }
// }