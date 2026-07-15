#pragma once
#include <optional>
#include <string>
#include "asset/metadata/AssetMetadataName.h"
#include "external/json.hpp"

// Pure id->id mapping. No behavior, no rendering/collision data of its
// own — just says which asset ids a given entity type pulls from each
// component repository. Add a field here when a second asset-backed
// component type needs archetype wiring (second-pressure rule applies
// to this struct too).
struct ArchetypeDefinition : public AssetMetadataName {
    std::optional<std::string> renderId;
    std::optional<std::string> collisionId;
    std::optional<std::string> animationId;     // packed-strip assets (AnimationAssetMetadata)
    std::optional<std::string> pmdAnimationId;  // PMD-style assets (PmdAnimationSetMetadata)

    static ArchetypeDefinition fromJson(const std::string& id, const nlohmann::json& entry);
};