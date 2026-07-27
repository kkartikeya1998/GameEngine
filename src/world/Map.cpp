#include "game/world/Map.h"
#include "system/GameConstants.h"
#include "engine/utility/Assert.h"

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
    ENGINE_ASSERT(x >= 0 && x < width && y >= 0 && y < height);
    return static_cast<std::size_t>(y * width + x);
}

Tile& Map::tile_at(int x, int y) {
    return tiles[index(x, y)];
}

const Tile& Map::tile_at(int x, int y) const {
    return tiles[index(x, y)];
}

void Map::set_tile(int x, int y, Tile tile) {
    tiles[index(x, y)] = Tile(std::move(tile));
}

bool Map::isWalkable(const AABB& box) const {
    return isPassable(box, MovementMode::Walking);
}

bool Map::isPassable(const AABB& box, MovementMode allowedModes) const {
    int startX = static_cast<int>(box.x) / GameConstants::TILE_SIZE;
    int startY = static_cast<int>(box.y) / GameConstants::TILE_SIZE;
    int endX = static_cast<int>(box.x + box.width - 0.01f) / GameConstants::TILE_SIZE;
    int endY = static_cast<int>(box.y + box.height - 0.01f) / GameConstants::TILE_SIZE;

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            const Tile& t = tile_at(x, y);

            if (t.getTerrain() == TerrainType::Wall)
                return false; // walls block every mode, including flight

            if (t.getTerrain() == TerrainType::Water) {
                if (!any(allowedModes & (MovementMode::Surfing | MovementMode::Flying)))
                    return false;
                continue;
            }

            if (!any(t.getMovementFlags() & allowedModes))
                return false;
        }
    }
    return true;
}