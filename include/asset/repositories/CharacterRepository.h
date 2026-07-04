#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <unordered_map>
#include <optional>

#include "system/GameConstants.h"
#include "asset/metadata/CollisionBox.h"
#include "asset/metadata/AssetMetadata.h"
#include "asset/metadata/RenderBox.h"
#include "asset/repositories/IAssetRepository.h"

struct CharacterMetadata : public AssetMetadata {
    RenderBox renderBox;

    std::optional<CollisionBox> collisionBox;
};
class CharacterRepository : public IAssetRepository {
public:
    explicit CharacterRepository(const std::filesystem::path& metadataFilePath);

    const CharacterMetadata* find(const std::string& frameName) const;

private:
    std::unordered_map<std::string, CharacterMetadata> frames_;

    void load_from_file(const std::filesystem::path& path);
};