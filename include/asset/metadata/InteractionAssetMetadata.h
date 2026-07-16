#pragma once
#include <string>
#include <vector>
#include "asset/metadata/AssetMetadataName.h"
#include "external/json.hpp"

enum class InteractionStepType { Dialogue, Animation, Battle, Wait };

struct InteractionStepData {
    InteractionStepType type;
    std::string text;         // Dialogue
    std::string animationId;  // Animation
    std::string targetEntity; // Animation: "self" or an entity tag
    std::string trainerId;    // Battle
    float waitSeconds = 0.f;  // Wait
};

struct InteractionAssetMetadata : public AssetMetadataName {
    std::vector<InteractionStepData> steps;
    static InteractionAssetMetadata fromJson(const std::string& id, const nlohmann::json& entry);
};