#include <fstream>

#include "asset/repositories/TileRepository.h"
#include "external/json.hpp"
#include "asset/metadata/MetadataParser.h"
#include "system/GameConstants.h"

using json = nlohmann::json;

TileRepository::TileRepository(
    const std::filesystem::path &metadataFilePath)
{
    load_from_file(metadataFilePath);
}

void TileRepository::load_from_file(
    const std::filesystem::path &path)
{
    std::ifstream file(path);

    if (!file)
    {
        // std::cerr << "Failed to open tile metadata file: "
        //           << path << '\n';
        return;
    }

    json j;
    file >> j;

    const auto &tilesets = j.at("tilesets");

    for (auto tilesetIt = tilesets.begin();
         tilesetIt != tilesets.end();
         ++tilesetIt)
    {
        const std::string tilesetName = tilesetIt.key();
        const json &tileset = tilesetIt.value();

        const std::string texture =
            tileset.value("texture", "");

        int tileSize =
            tileset.value("tile_size",
                          GameConstants::TILE_SIZE);

        const auto &tileTypes =
            tileset.at("tile_types");

        for (auto it = tileTypes.begin();
             it != tileTypes.end();
             ++it)
        {
            const std::string tileType = it.key();
            const json &def = it.value();

            TileTypeMetadata meta;

            meta.name =
                tilesetName + "_" + tileType;

            const auto &render = def.at("render_component");
            meta.RenderData.textureRect = sf::IntRect{
                {render.value("x", 0), render.value("y", 0)},
                {render.value("w", 0), render.value("h", 0)}};
            meta.RenderData.texturePath = texture;
            meta.RenderData.sourceTileSize = tileSize;
            meta.RenderData.renderScale = def.value("scale", RenderData::autoScale(tileSize));
            types_.emplace(meta.name, std::move(meta));
        }
    }
}

const TileTypeMetadata *TileRepository::find(
    const std::string &type) const
{
    auto it = types_.find(type);
    return it == types_.end() ? nullptr : &it->second;
}