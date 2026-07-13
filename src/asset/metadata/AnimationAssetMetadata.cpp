#include "asset/metadata/AnimationAssetMetadata.h"

AnimationAssetMetadata AnimationAssetMetadata::fromJson(const std::string &id, const nlohmann::json &entry)
{
    AnimationAssetMetadata meta;
    meta.name = id;
    meta.texturePath = entry.value("texture", "");
    meta.frameWidth = entry.value("frame_width", 0);
    meta.frameHeight = entry.value("frame_height", 0);
    meta.columns = entry.value("columns", 1);
    meta.frameDuration = entry.value("frame_duration", 0.1f);
    meta.loop = entry.value("loop", true);

    for (auto it = entry["directions"].begin(); it != entry["directions"].end(); ++it)
    {
        AnimationDirectionData dir;
        dir.startIndex = it.value().value("start_index", 0);
        dir.frameCount = it.value().value("frame_count", 1);
        meta.directions.emplace(it.key(), dir);
    }
    return meta;
}

sf::IntRect AnimationAssetMetadata::frameRect(const std::string &direction, int frameIndex) const
{
    auto it = directions.find(direction);
    const AnimationDirectionData &dir = (it != directions.end()) ? it->second : directions.at("down");

    int clamped = loop ? frameIndex % dir.frameCount : std::min(frameIndex, dir.frameCount - 1);
    int linearIdx = dir.startIndex + clamped;
    int row = linearIdx / columns;
    int col = linearIdx % columns;

    return sf::IntRect{{col * frameWidth, row * frameHeight}, {frameWidth, frameHeight}};
}