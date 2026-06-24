#include "asset/MapObjectRepository.h"

#include <fstream>
#include <iostream>
#include "external/json.hpp"

using json = nlohmann::json;

MapObjectRepository::MapObjectRepository(const std::string& metadataFilePath) {
    load_from_file(metadataFilePath);
}

void MapObjectRepository::load_from_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open object metadata file: " << path << '\n';
        return;
    }

    json j;
    file >> j;

    const auto& object_types = j.at("object_types");
    for (auto it = object_types.begin(); it != object_types.end(); ++it) {
        const std::string& typeName = it.key();
        const json& def = it.value();

        ObjectTypeMetadata meta;
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

        for (const auto& cell : def.at("footprint")) {
            FootprintCell fc;
            fc.dx = cell.value("dx", 0);
            fc.dy = cell.value("dy", 0);
            fc.blocking = cell.value("blocking", false);
            fc.isTeleportTile = cell.value("teleport", false);
            meta.footprint.push_back(fc);
        }

        types_.emplace(typeName, std::move(meta));
    }
}

void MapObjectRepository::register_type(const std::string& typeName, ObjectTypeMetadata meta) {
    types_.emplace(typeName, std::move(meta));
}

const ObjectTypeMetadata* MapObjectRepository::find(const std::string& type) const {
    auto it = types_.find(type);
    return it == types_.end() ? nullptr : &it->second;
}