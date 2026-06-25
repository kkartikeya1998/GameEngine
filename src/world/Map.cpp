#include "world/Map.h"

#include <cmath>
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

void Map::set_tile(int x, int y, Terrain::Type terrain, std::string typeName) {
    tiles[index(x, y)] = Tile(terrain, std::move(typeName));
}

void Map::addMapObject(std::unique_ptr<MapObject> obj) {
    map_objects.push_back(std::move(obj));
}

bool Map::isAreaBlocked(const AABB& box, float tileSize) const {
    // CHANGED: std::floor instead of static_cast<int>. static_cast<int>
    // truncates toward zero, so for any negative coordinate with
    // magnitude less than one tileSize (e.g. box.x == -15.8, tileSize ==
    // 64), -15.8 / 64 == -0.247, and static_cast<int>(-0.247) == 0 — NOT
    // -1. That silently mapped an out-of-bounds position to tile index 0
    // (a valid, in-range tile), so the bounds check below never
    // triggered until the box drifted past a full extra tileSize
    // further out. std::floor rounds toward negative infinity, so
    // floor(-0.247) == -1.0, correctly landing outside the valid range
    // immediately.
    //
    // Only check the small neighborhood of tiles the box could possibly
    // overlap, not the whole map — fine at any reasonable map size,
    // since this runs every frame per moving entity.
    int minTileX = static_cast<int>(std::floor(box.x / tileSize));
    int maxTileX = static_cast<int>(std::floor((box.x + box.width) / tileSize));
    int minTileY = static_cast<int>(std::floor(box.y / tileSize));
    int maxTileY = static_cast<int>(std::floor((box.y + box.height) / tileSize));

    for (int ty = minTileY; ty <= maxTileY; ++ty) {
        for (int tx = minTileX; tx <= maxTileX; ++tx) {
            if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
                return true; // out of bounds counts as blocked
            }
            if (!tile_at(tx, ty).isWalkable()) {
                AABB tileBox{
                    static_cast<float>(tx) * tileSize,
                    static_cast<float>(ty) * tileSize,
                    tileSize,
                    tileSize
                };
                if (box.intersects(tileBox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// void Map::render() const {
//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             std::cout << tile_at(x, y).render_char();
//         }
//         std::cout << '\n';
//     }
// }