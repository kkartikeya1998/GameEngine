#include "asset/metadata/RenderAssetMetadata.h"
#include "system/GameConstants.h"

RenderAssetMetadata RenderAssetMetadata::fromJson(const std::string& id, const nlohmann::json& entry)
{
    RenderAssetMetadata meta;
    meta.name = id;

    meta.RenderData.texturePath = entry.value("texture", "");
    meta.RenderData.sourceTileSize = entry.value("tile_size", GameConstants::TILE_SIZE);
    meta.RenderData.textureRect = sf::IntRect{
        { entry.value("x", 0), entry.value("y", 0) },
        { entry.value("w", 0), entry.value("h", 0) }
    };

    return meta;
}