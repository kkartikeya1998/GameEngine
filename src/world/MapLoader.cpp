#include "world/MapLoader.h"
#include "world/Map.h"
#include "log/Logger.h"
#include "entities/npc/TileRestrictionComponent.h"
#include "entities/pokemon/WildPokemon.h"
#include "system/GameConstants.h"
#include "entities/EntityFactory.h"
#include "asset/AsssetPaths.h"
#include "component/WorldItemComponent.h"
#include "component/InteractableComponent.h"

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

        maps_metadata_.push_back({id, entry.path().string()});
    }
}

void MapLoader::loadInto(Registry &registry, Map &map, int mapId) const
{
    LOG_INFO(std::format("Loading map ID: {}", mapId));
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
    {
        LOG_ERROR(std::format("Map ID {} not found in metadata", mapId));
        throw std::runtime_error("Map not found");
    }

    LOG_DEBUG(std::format("Loading map file: {}", mapPath));
    std::ifstream file(mapPath);

    json j;
    file >> j;

    int width = j["width"];
    int height = j["height"];
    LOG_INFO(std::format("Map loaded: {} ({}x{})", mapPath, width, height));
    map = Map(width, height);

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
            RenderComponent tileRender(
                typeName,
                tileMeta->RenderData.texturePath,
                tileMeta->RenderData.textureRect,
                tileMeta->RenderData.sourceTileSize,
                static_cast<float>(x * GameConstants::TILE_SIZE),
                static_cast<float>(y * GameConstants::TILE_SIZE),
                RenderLayer::Terrain,
                tileMeta->RenderData.renderScale);

            tile.addRenderComponent(tileRender);
            map.set_tile(x, y, tile);
        }
    }

    if (j.contains("map_objects"))
    {
        for (const auto &entry : j["map_objects"])
        {
            std::string archetype = entry["type"];
            float originX = entry["origin"]["x"];
            float originY = entry["origin"]["y"];

            EntityID id = registry.create();
            try
            {
                EntityFactory::populate(registry, assets_, id, archetype, originX, originY, RenderLayer::Characters);
            }
            catch (const std::runtime_error &)
            {
                registry.destroy(id); // unknown archetype: don't leave a dangling empty entity
            }
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

            EntityID wildId = makeWildPokemon(registry, speciesId, level, originX, originY, assets_, std::move(allowedTiles));

            // Per-instance override: replaces whatever InteractableComponent the
            // archetype set, so only THIS map entry triggers the gift interaction —
            // other species_1 spawns elsewhere keep the archetype default.
            if (entry.contains("giftInteraction"))
                registry.add<InteractableComponent>(wildId, entry["giftInteraction"].get<std::string>());
        }
    }

    if (j.contains("item_pickups"))
    {
        for (const auto &entry : j["item_pickups"])
        {
            std::string archetype = entry["archetype"]; // visuals/collision only
            std::string itemId = entry["itemId"];
            int quantity = entry.value("quantity", 1);
            float originX = entry["origin"]["x"];
            float originY = entry["origin"]["y"];

            EntityID id = registry.create();
            try
            {
                EntityFactory::populate(registry, assets_, id, archetype, originX, originY, RenderLayer::Characters);
                registry.add<WorldItemComponent>(id, itemId, quantity);
            }
            catch (const std::runtime_error &)
            {
                registry.destroy(id);
            }
        }
    }
}