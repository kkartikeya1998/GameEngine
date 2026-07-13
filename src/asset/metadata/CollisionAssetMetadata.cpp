#include "asset/metadata/CollisionAssetMetadata.h"

CollisionAssetMetadata CollisionAssetMetadata::fromJson(const std::string& id, const nlohmann::json& entry)
{
    CollisionAssetMetadata meta;
    meta.name = id;
    meta.data.offsetX = entry.value("offset_x", 0.f);
    meta.data.offsetY = entry.value("offset_y", 0.f);
    meta.data.width   = entry.value("width", 0.f);
    meta.data.height  = entry.value("height", 0.f);
    return meta;
}