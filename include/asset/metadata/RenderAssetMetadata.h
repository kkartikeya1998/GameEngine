#pragma once
#include "asset/metadata/AssetMetadataName.h"
#include <SFML/Graphics.hpp>
#include "external/json.hpp"
#include "system/GameConstants.h"

struct RenderData
{
    std::string name;

    std::string texturePath;

    sf::IntRect textureRect;

    int sourceTileSize = 32;
    float renderScale = 1.0f;
    
    static float autoScale(int sourceTileSizePx)
    {
        return sourceTileSizePx > 0
                   ? GameConstants::TILE_SIZE / static_cast<float>(sourceTileSizePx)
                   : 1.0f;
    }
};

struct RenderAssetMetadata : public AssetMetadataName
{
    RenderData RenderData;

    static RenderAssetMetadata fromJson(const std::string &id, const nlohmann::json &entry);
};