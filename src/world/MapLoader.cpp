#include "world/MapLoader.h"
#include "world/Map.h"
#include "entities/npc/TileRestrictionComponent.h"
#include "entities/pokemon/WildPokemon.h"
#include "system/GameConstants.h"
#include "entities/EntityFactory.h"
#include "asset/AsssetPaths.h"

#include <cmath>
#include <cstdlib>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "external/json.hpp"

using json = nlohmann::json;

MapLoader::MapLoader(const AssetDatabase &assets)
    : assets_(assets)
{
    loadMetadata();
}

void MapLoader::loadMetadata()
{
    namespace fs = std::filesystem;

    maps_metadata_.clear();

    for (const auto &entry : fs::directory_iterator(Assets::MAPS))
    {
        if (entry.path().extension() != ".json")
            continue;

        if (entry.path().filename() == "object_metadata.json")
            continue;

        std::ifstream file(entry.path());
        if (!file)
            continue;

        json j;
        file >> j;

        int id = j.value("id", -1);

        maps_metadata_.push_back({id,
                                  entry.path().string()});
    }
}

std::unique_ptr<Map> MapLoader::loadMapById(int mapId) const
{
    std::string mapPath;

    for (const auto &meta : maps_metadata_)
    {
        if (meta.id == mapId)
        {
            mapPath = meta.path;
            break;
        }
    }

    if (mapPath.empty())
        throw std::runtime_error("Map not found");

    std::ifstream file(mapPath);

    json j;
    file >> j;

    int width = j["width"];
    int height = j["height"];
    auto map = std::make_unique<Map>(width, height);

    const auto &tiles_json = j["tiles"];

    if (tiles_json.size() != static_cast<std::size_t>(width * height))
    {
        throw std::runtime_error("Tile count does not match width * height");
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int flat = y * width + x;
            std::string typeName = tiles_json[flat].get<std::string>();

            Tile tile;
            const RenderAssetMetadata *tileMeta = assets_.findRender(typeName);
            tile.addRenderComponent(RenderComponent(
                typeName,
                tileMeta->RenderData.texturePath,
                tileMeta->RenderData.textureRect,
                tileMeta->RenderData.sourceTileSize,
                static_cast<float>(x * GameConstants::TILE_SIZE),
                static_cast<float>(y * GameConstants::TILE_SIZE)));
            map->set_tile(
                x,
                y,
                tile);
        }
    }

    if (j.contains("map_objects"))
    {
        for (const auto &entry : j["map_objects"])
        {
            std::string archetype = entry["type"];
            float originX = entry["origin"]["x"];
            float originY = entry["origin"]["y"];

            auto entity = EntityFactory::make(assets_, archetype, originX, originY, RenderLayer::Characters);
            if (entity)
                map->addMapObject(std::move(entity));
        }
    }

    if (j.contains("wild_pokemon"))
    {
        for (const auto &entry : j["wild_pokemon"])
        {
            int speciesId = entry["speciesId"];
            int levelMin = entry.value("level_min", entry.value("level", 5));
            int levelMax = entry.value("level_max", levelMin);
            int level = levelMin;
            if (levelMax > levelMin)
                level += std::rand() % (levelMax - levelMin + 1);

            float originX = entry["origin"]["x"];
            float originY = entry["origin"]["y"];

            std::unordered_set<TileCoord, TileCoordHash> allowedTiles;
            if (entry.contains("allowedTiles"))
                for (const auto &t : entry["allowedTiles"])
                    allowedTiles.insert(TileCoord{t["x"].get<int>(), t["y"].get<int>()});

            auto entity = std::make_unique<Entity>(
                makeWildPokemon(speciesId, level, originX, originY, assets_, std::move(allowedTiles)));

            map->addNpc(std::move(entity));
        }
    }
    return map;
}