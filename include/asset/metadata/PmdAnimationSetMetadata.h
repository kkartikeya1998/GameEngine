#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "asset/metadata/AssetMetadataName.h"
#include "external/json.hpp"

// One clip (Walk, Attack, Idle...) within a PMD-style sprite set.
// Row = direction (fixed order, see PmdDirectionOrder.h), column = frame.
struct PmdClipMetadata {
    std::string texturePath;
    int frameWidth = 0;
    int frameHeight = 0;
    int frameCount = 0;                 // columns; same across all direction rows
    std::vector<float> frameDurations;  // seconds, size == frameCount
    bool loop = true;

    sf::IntRect frameRect(int directionRow, int frameIndex) const {
        return sf::IntRect{
            {frameIndex * frameWidth, directionRow * frameHeight},
            {frameWidth, frameHeight}
        };
    }

    static PmdClipMetadata fromJson(const nlohmann::json& j) {
        PmdClipMetadata clip;
        clip.texturePath = j.at("texturePath").get<std::string>();
        clip.frameWidth = j.at("frameWidth").get<int>();
        clip.frameHeight = j.at("frameHeight").get<int>();
        clip.frameCount = j.at("frameCount").get<int>();
        clip.loop = j.value("loop", true);
        for (const auto& d : j.at("frameDurations"))
            clip.frameDurations.push_back(d.get<float>());
        return clip;
    }
};

// One per creature/character: id -> its available named clips.
struct PmdAnimationSetMetadata : public AssetMetadataName {
    std::unordered_map<std::string, PmdClipMetadata> clips;

    static PmdAnimationSetMetadata fromJson(const std::string& id, const nlohmann::json& entry) {
        PmdAnimationSetMetadata set;
        set.name = id;
        for (auto it = entry.begin(); it != entry.end(); ++it)
            set.clips.emplace(it.key(), PmdClipMetadata::fromJson(it.value()));
        return set;
    }
};