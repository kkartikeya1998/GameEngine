#include "world/Map.h"

#include <fstream>
#include <iostream>

#include "external/json.hpp"

using json = nlohmann::json;

namespace {
Terrain::Type terrain_from_string(const std::string& s) {
    if (s == "Grass") return Terrain::Type::Grass;
    if (s == "Sand") return Terrain::Type::Sand;
    if (s == "Water") return Terrain::Type::Water;
    if (s == "Ice") return Terrain::Type::Ice;
    if (s == "Cave") return Terrain::Type::Cave;
    if (s == "Building") return Terrain::Type::Building;
    return Terrain::Type::Grass;
}
}

Map::Map(const std::string& path) {
    load_from_file(path);
}

std::size_t Map::index(int x, int y) const {
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(width)
         + static_cast<std::size_t>(x);
}

Tile& Map::tile_at(int x, int y) {
    return tiles.at(index(x, y));
}

const Tile& Map::tile_at(int x, int y) const {
    return tiles.at(index(x, y));
}

void Map::set_tile(int x, int y, Terrain::Type terrain) {
    tile_at(x, y) = Tile(terrain);
}

void Map::load_from_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open map file: " << path << '\n';
        width = 0;
        height = 0;
        tiles.clear();
        map_objects.clear();
        return;
    }

    json j;
    file >> j;

    width = j.value("width", 0);
    height = j.value("height", 0);

    tiles.assign(static_cast<std::size_t>(width * height), Tile{});

    const auto& tiles_json = j["tiles"];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const int flat = y * width + x;
            tiles[static_cast<std::size_t>(flat)] =
                Tile(terrain_from_string(tiles_json[flat].get<std::string>()));
        }
    }
}

// void Map::render() const {
//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             std::cout << tile_at(x, y).render_char();
//         }
//         std::cout << '\n';
//     }
// }