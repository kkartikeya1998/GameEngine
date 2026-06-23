#pragma once

#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "world/Terrain.h"

struct TileTypeMetadata {
    std::string texturePath;
    sf::IntRect textureRect;
    Terrain::Type terrainType;
};

class MapRepository {
public:
    MapRepository(const std::string& metadataFilePath);

    const TileTypeMetadata* find(const std::string& type) const;

private:
    void load_from_file(const std::string& path);
    std::unordered_map<std::string, TileTypeMetadata> types_;
};