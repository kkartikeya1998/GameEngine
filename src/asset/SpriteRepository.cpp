#include "asset/SpriteRepository.h"

#include <fstream>
#include <iostream>
#include "external/json.hpp"

using json = nlohmann::json;

SpriteRepository::SpriteRepository(const std::string& metadataFilePath) {
    load_from_file(metadataFilePath);
}

void SpriteRepository::load_from_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open sprite metadata file: " << path << '\n';
        return;
    }

    json j;
    file >> j;
    float tile_size = j.value("tile_size", 0);
    const auto& frames = j.at("frames");
    for (auto it = frames.begin(); it != frames.end(); ++it) {
        const std::string& frameName = it.key();
        const json& def = it.value();

        SpriteFrameMetadata meta;
        meta.spriteName = frameName;
        meta.texturePath = def.value("texture", "");

        int x = def.value("x", 0);
        int y = def.value("y", 0);
        int w = def.value("w", 0);
        int h = def.value("h", 0);

        meta.textureRect = sf::IntRect(
            sf::Vector2<int>(x, y),
            sf::Vector2<int>(w, h)
        );
        meta.sourceTileSize = tile_size;

        frames_.emplace(frameName, meta);
    }
}

const SpriteFrameMetadata* SpriteRepository::find(const std::string& frameName) const {
    auto it = frames_.find(frameName);
    return it == frames_.end() ? nullptr : &it->second;
}