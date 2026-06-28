#include "world/Map.h"

#include <cmath>
#include <iostream>
#include <optional>
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

bool Map::isAreaBlocked(const AABB& box) const {

    int minTileX = static_cast<int>(std::floor(box.x / GameConstants::TILE_SIZE));
    int maxTileX = static_cast<int>(std::floor((box.x + box.width) / GameConstants::TILE_SIZE));
    int minTileY = static_cast<int>(std::floor(box.y / GameConstants::TILE_SIZE));
    int maxTileY = static_cast<int>(std::floor((box.y + box.height) / GameConstants::TILE_SIZE));

    for (int ty = minTileY; ty <= maxTileY; ++ty) {
        for (int tx = minTileX; tx <= maxTileX; ++tx) {
            if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
                return true; // out of bounds counts as blocked
            }
            if (!tile_at(tx, ty).isWalkable()) {
                AABB tileBox{
                    static_cast<float>(tx) * GameConstants::TILE_SIZE,
                    static_cast<float>(ty) * GameConstants::TILE_SIZE,
                    GameConstants::TILE_SIZE,
                    GameConstants::TILE_SIZE
                };
                if (box.intersects(tileBox)) {
                    return true;
                }
            }
        }
    }

    // Second, independent collision source: objects with a precise
    // collisionBox (see CollisionBox in MapObjectRepository.h). This is
    // deliberately separate from the tile-grid check above — an object
    // with a collisionBox does NOT also mark its tile as blocking via
    // footprint (see MapLoader::applyFootprint), so there's no double
    // application of blocking for the same object. Objects WITHOUT a
    // collisionBox return nullopt here and rely entirely on the
    // tile-grid check above instead, via their footprint's blocking
    // flag — exactly as before this method existed.
    for (const auto& obj : map_objects) {
        std::optional<AABB> objBox = obj->getCollisionBox();
        if (objBox && box.intersects(*objBox)) {
            return true;
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