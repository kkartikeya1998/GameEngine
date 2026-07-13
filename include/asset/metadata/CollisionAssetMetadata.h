#pragma once
#include "asset/metadata/AssetMetadataName.h"
#include "asset/metadata/CollisionData.h"
#include "external/json.hpp"

struct CollisionAssetMetadata : public AssetMetadataName {
    CollisionData data;

    static CollisionAssetMetadata fromJson(const std::string& id, const nlohmann::json& entry);
};