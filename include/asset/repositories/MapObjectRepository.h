#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>
#include "system/GameConstants.h"
#include "asset/repositories/IAssetRepository.h"
#include "asset/metadata/CollisionBox.h"
#include "asset/metadata/RenderBox.h"
#include "asset/metadata/AssetMetadata.h"
#include "asset/AsssetPaths.h"



struct ObjectTypeMetadata : public AssetMetadata {
    RenderBox renderBox;
    std::optional<CollisionBox> collisionBox;
};


class MapObjectRepository : public IAssetRepository {
public:
    explicit MapObjectRepository(const std::filesystem::path& metadataFilePath);

    // void register_type(const std::string& typeName, ObjectTypeMetadata meta);

    const ObjectTypeMetadata* find(const std::string& type) const;

private:
    void load_from_file(const std::filesystem::path& path);
    std::unordered_map<std::string, ObjectTypeMetadata> types_;
};