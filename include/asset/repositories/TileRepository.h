#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "asset/repositories/IAssetRepository.h"
#include "asset/metadata/AssetMetadataName.h"
#include "asset/metadata/RenderData.h"
#include "system/GameConstants.h"
// ---------------------------------------------------------------------------
// TileTypeMetadata — full description of one tile TYPE.
// ---------------------------------------------------------------------------
struct TileTypeMetadata : public AssetMetadataName {
    
    RenderData RenderData;
};

// ---------------------------------------------------------------------------
// TileRepository — owns all tile type metadata, loaded once from a JSON
// file at construction. Derives from IAssetRepository so AssetManager can
// own and serve it alongside the other metadata repositories.
// ---------------------------------------------------------------------------
class TileRepository : public IAssetRepository {
public:
    explicit TileRepository(const std::filesystem::path& metadataFilePath);

    const TileTypeMetadata* find(const std::string& type) const;

private:
    void load_from_file(const std::filesystem::path& path);
    std::unordered_map<std::string, TileTypeMetadata> types_;
};