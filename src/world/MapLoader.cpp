#include "world/MapLoader.h"
#include "world/Map.h"
#include "entities/Entity.h"
#include "tmp/movement/PositionComponent.h"
#include "tmp/movement/CollisionComponent.h"
#include "tmp/movement/RenderComponent.h"
#include "system/GameConstants.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "external/json.hpp"

using json = nlohmann::json;

MapLoader::MapLoader(
    MapObjectRepository &mapObjectRepository, TileRepository &tileRepository) : 
        mapObjectRepository_(mapObjectRepository),
        tileRepository_(tileRepository)
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
    std::cout << "[MapLoader]: Loading map with ID: " << mapId << "\n";
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
    std::cout << "[MapLoader]: Map dimensions: " << width << "x" << height << "\n";
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
            TileTypeMetadata const *tileMeta = tileRepository_.find(typeName);

            // probably shifted half a tile size to the right and down, 
            tile.addRenderComponent(RenderComponent(
                typeName,
                tileMeta->renderBox.texturePath,
                tileMeta->renderBox.textureRect,
                tileMeta->renderBox.sourceTileSize,
                static_cast<float>(x * GameConstants::TILE_SIZE),
                static_cast<float>(y * GameConstants::TILE_SIZE)
            ));
            map->set_tile(
                x,
                y,
                tile);
        }
    }

    std::cout << "[MapLoader]: Map tiles loaded.\n";
    if (j.contains("map_objects"))
    {
        for (const auto &entry : j["map_objects"])
        {
            std::string type = entry["type"];

            const ObjectTypeMetadata *meta = mapObjectRepository_.find(type);
            if (!meta)
                continue;

            float originX = entry["origin"]["x"];
            float originY = entry["origin"]["y"];

            auto entity = std::make_unique<Entity>();

            entity->add<PositionComponent>(
                originX,
                originY);

            entity->add<RenderComponent>(
                type,
                meta->renderBox.texturePath,
                meta->renderBox.textureRect,
                meta->renderBox.sourceTileSize,
                originX,
                originY);

            if (meta->collisionBox.has_value())
            {
                const CollisionBox &cb = *meta->collisionBox;
                float scale = static_cast<float>(GameConstants::TILE_SIZE) / static_cast<float>(meta->renderBox.sourceTileSize);

                entity->add<CollisionComponent>(
                    cb.offsetX * scale,
                    cb.offsetY * scale,
                    cb.width * scale,
                    cb.height * scale);
            }

            map->addMapObject(std::move(entity));
        }
    }

    return map;
}