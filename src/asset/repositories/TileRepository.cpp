#include "asset/repositories/TileRepository.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "external/json.hpp"

using json = nlohmann::json;

TileRepository::TileRepository(const std::filesystem::path& metadataFilePath) {
    load_from_file(metadataFilePath);
}

void TileRepository::load_from_file(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open tile metadata file: " << path << '\n';
        return;
    }

    json j;
    file >> j;

    const auto& tile_types = j.at("tile_types");
    for (auto it = tile_types.begin(); it != tile_types.end(); ++it) {
        const std::string& typeName = it.key();
        const json& def = it.value();

        TileTypeMetadata meta;
        meta.name = typeName;
        meta.texturePath = def.value("texture", "");

        int x = def.value("x", 0);
        int y = def.value("y", 0);
        int w = def.value("w", 0);
        int h = def.value("h", 0);

        meta.textureRect = sf::IntRect(
            sf::Vector2<int>(x, y),
            sf::Vector2<int>(w, h)
        );

        meta.terrainType = def.value("terrain", "Grass");

        // Defaults to w (the region's own width) if not explicitly given —
        // for a tile, "pixels per tile" and "this region's width" are the
        // same thing unless you say otherwise. Falls back further to GameConstants::TILE_SIZE
        // only if w itself was never set (malformed/legacy entry).
        meta.sourceTileSize = def.value("tile_size", w > 0 ? w : GameConstants::TILE_SIZE);

        types_.emplace(typeName, std::move(meta));
    }
}

const TileTypeMetadata* TileRepository::find(const std::string& type) const {
    auto it = types_.find(type);
    return it == types_.end() ? nullptr : &it->second;
}