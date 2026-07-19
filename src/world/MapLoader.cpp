#include "world/MapLoader.h"
#include "world/Map.h"
#include "log/Logger.h"
#include "entities/npc/TileRestrictionComponent.h"
#include "entities/pokemon/WildPokemon.h"
#include "system/GameConstants.h"
#include "entities/EntityFactory.h"
#include "asset/AsssetPaths.h"
#include "exceptions/EngineExceptions.h"
#include "component/WorldItemComponent.h"
#include "component/InteractableComponent.h"

#include <cmath>
#include <cstdlib>
#include <unordered_set>
#include <filesystem>
#include <fstream>

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
        try
        {
            file >> j;
        }
        catch (const json::parse_error &e)
        {
            // One malformed map metadata file shouldn't prevent every other
            // map from being discovered — log and keep scanning.
            LOG_ERROR(std::format("MapLoader: malformed map metadata file {} — {}", entry.path().string(), e.what()));
            continue;
        }

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
        // Every map transition target comes from validated game logic, not
        // player-controllable input — a missing id here means the engine's
        // own state is inconsistent (a transition pointing at a map that
        // doesn't exist), not a piece of bad content a player can trigger.
        // Fatal rather than guessing a fallback map, which would silently
        // strand the player somewhere unintended.
        LOG_ERROR(std::format("Map ID {} not found in metadata", mapId));
        throw EngineException(std::format("MapLoader: map id {} not found", mapId));
    }

    LOG_DEBUG(std::format("Loading map file: {}", mapPath));
    std::ifstream file(mapPath);
    if (!file)
        throw EngineException("MapLoader: failed to open map file: " + mapPath);

    json j;
    try
    {
        file >> j;
    }
    catch (const json::parse_error &e)
    {
        throw EngineException(std::format("MapLoader: malformed map file {} — {}", mapPath, e.what()));
    }

    int width = j["width"];
    int height = j["height"];
    LOG_INFO(std::format("Map loaded: {} ({}x{})", mapPath, width, height));
    map = Map(width, height);

    const auto &tiles_json = j["tiles"];

    if (tiles_json.size() != static_cast<std::size_t>(width * height))
    {
        throw EngineException("MapLoader: tile count does not match width * height in " + mapPath);
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int flat = y * width + x;
            std::string typeName = tiles_json[flat].get<std::string>();

            const RenderAssetMetadata *tileMeta = assets_.findRender(typeName);
            if (!tileMeta)
            {
                // FIX: this used to dereference tileMeta unconditionally,
                // crashing on any typo'd tile id in map JSON. A bad tile is
                // recoverable — leave it blank (map.set_tile is simply
                // skipped) and keep loading the rest of the map. A visibly
                // blank tile is far easier to spot and fix in content than
                // a crash on load.
                LOG_ERROR(std::format("MapLoader: unknown tile render id '{}' at ({},{}) in {} — leaving tile blank",
                                       typeName, x, y, mapPath));
                continue;
            }

            Tile tile;
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
            Result<void, AssetError> result =
                EntityFactory::populate(registry, assets_, id, archetype, originX, originY, RenderLayer::Characters);
            if (!result)
            {
                LOG_ERROR(std::format("MapLoader: skipping map_object in {} — {}", mapPath, result.error().toString()));
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

            // NOT ADDRESSED: makeWildPokemon() has no error handling here,
            // and I don't have WildPokemon.cpp to know whether/how it can
            // fail on a bad speciesId — guessing its signature would break
            // the "never invent signatures" rule. Share that file and I'll
            // bring it in line with the same Result pattern.
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
            Result<void, AssetError> result =
                EntityFactory::populate(registry, assets_, id, archetype, originX, originY, RenderLayer::Characters);
            if (!result)
            {
                LOG_ERROR(std::format("MapLoader: skipping item_pickup '{}' in {} — {}", itemId, mapPath, result.error().toString()));
                registry.destroy(id);
                continue;
            }
            registry.add<WorldItemComponent>(id, itemId, quantity);
        }
    }
}