#include "world/MapLoader.h"
#include "world/Terrain.h"
#include "world/Map.h"
#include "entities/Entity.h"
#include "tmp/movement/PositionComponent.h"
#include "tmp/movement/CollisionComponent.h"
#include "world/MapObjectRenderComponent.h"
#include "system/GameConstants.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "external/json.hpp"

using json = nlohmann::json;

MapLoader::MapLoader(
    MapObjectRepository &repository) : repository_(repository)
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

// void MapLoader::applyFootprint(Map& map, const ObjectTypeMetadata& meta,
//                                 int originX, int originY) const
// {
//     const int width = map.getWidth();
//     const int height = map.getHeight();

//     int originTileX = static_cast<int>(std::floor(originX / GameConstants::TILE_SIZE));
//     int originTileY = static_cast<int>(std::floor(originY / GameConstants::TILE_SIZE));

//     bool hasOwnCollisionBox = meta.collisionBox.has_value();

//     for (const auto& fc : meta.footprint)
//     {
//         int tileX = originTileX + fc.dx;
//         int tileY = originTileY + fc.dy;

//         if (tileX < 0 || tileX >= width || tileY < 0 || tileY >= height)
//             continue;

//         Tile& tile = map.tile_at(tileX, tileY);

//         if (fc.blocking && !hasOwnCollisionBox) {
//             tile.setWalkable(std::make_unique<NoWalkable>());
//         }
//     }
// }

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

            map->set_tile(
                x,
                y,
                Terrain::terrain_from_string(typeName),
                typeName);
        }
    }

    if (j.contains("map_objects"))
    {
        for (const auto &entry : j["map_objects"])
        {
            std::string type = entry["type"];

            const ObjectTypeMetadata *meta = repository_.find(type);
            if (!meta)
                continue;

            float originX = entry["origin"]["x"];
            float originY = entry["origin"]["y"];

            // Remenent from the grid based maps implementation
            // applyFootprint(*map, *meta, originX, originY);

            auto entity = std::make_unique<Entity>();

            entity->add<PositionComponent>(
                originX,
                originY);

            entity->add<MapObjectRenderComponent>(
                meta,
                originX,
                originY);

            if (meta->collisionBox.has_value())
            {
                const CollisionBox &cb = *meta->collisionBox;
                float scale = static_cast<float>(GameConstants::TILE_SIZE) / static_cast<float>(meta->sourceTileSize);

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