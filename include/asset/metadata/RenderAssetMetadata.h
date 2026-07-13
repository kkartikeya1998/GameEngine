#pragma once
#include "asset/metadata/AssetMetadataName.h"
#include <SFML/Graphics.hpp>
#include "external/json.hpp"

struct RenderData {
    std::string name;
    
    std::string texturePath;

    sf::IntRect textureRect;

    int sourceTileSize = 32;
};

struct RenderAssetMetadata : public AssetMetadataName {
    RenderData RenderData;

    static RenderAssetMetadata fromJson(const std::string& id, const nlohmann::json& entry);
};