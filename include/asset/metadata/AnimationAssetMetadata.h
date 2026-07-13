#pragma once
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "asset/metadata/AssetMetadataName.h"
#include "external/json.hpp"

// AnimationAssetMetadata.h
struct AnimationDirectionData
{
    int startIndex = 0;
    int frameCount = 1;
};

struct AnimationAssetMetadata : public AssetMetadataName
{
    std::string texturePath;
    int frameWidth = 0;
    int frameHeight = 0;
    int columns = 1;
    float frameDuration = 0.1f;
    bool loop = true;
    std::unordered_map<std::string, AnimationDirectionData> directions;

    static AnimationAssetMetadata fromJson(const std::string &id, const nlohmann::json &entry);
    sf::IntRect frameRect(const std::string &direction, int frameIndex) const;
};