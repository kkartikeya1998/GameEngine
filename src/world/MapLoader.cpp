#include "world/MapLoader.h"
#include "world/Terrain.h"
#include "world/Map.h"
#include "entities/Entity.h"
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
    MapObjectRepository& repository
) : repository_(repository)
{
    loadMetadata();
}

void MapLoader::loadMetadata()
{
    namespace fs = std::filesystem;

    maps_metadata_.clear();

    for (const auto& entry : fs::directory_iterator(Assets::MAPS))
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

        maps_metadata_.push_back({
            id,
            entry.path().string()
        });
    }
}

void MapLoader::applyFootprint(Map& map, const ObjectTypeMetadata& meta,
                                int originX, int originY,
                                int teleportMapId, int teleportX, int teleportY) const
{
    const int width = map.getWidth();
    const int height = map.getHeight();


    int originTileX = static_cast<int>(std::floor(originX / GameConstants::TILE_SIZE));
    int originTileY = static_cast<int>(std::floor(originY / GameConstants::TILE_SIZE));


    bool hasOwnCollisionBox = meta.collisionBox.has_value();

    for (const auto& fc : meta.footprint)
    {
        int tileX = originTileX + fc.dx;
        int tileY = originTileY + fc.dy;

        if (tileX < 0 || tileX >= width || tileY < 0 || tileY >= height)
            continue;

        Tile& tile = map.tile_at(tileX, tileY);

        if (fc.blocking && !hasOwnCollisionBox) {
            tile.setWalkable(std::make_unique<NoWalkable>());
        }

        if (fc.isTeleportTile) {
            tile.setTeleportable(
                std::make_unique<StaticTeleport>(teleportMapId, teleportX, teleportY));
        }
    }
}

std::unique_ptr<Map> MapLoader::loadMapById(int mapId) const
{
    std::string mapPath;

    for (const auto& meta : maps_metadata_)
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

    const auto& tiles_json = j["tiles"];

    if (tiles_json.size() != static_cast<std::size_t>(width * height)) {
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
                typeName
            );
        }
    }

    if (j.contains("map_objects"))
    {
        for (const auto& entry : j["map_objects"])
        {
            std::string type = entry["type"];

            const ObjectTypeMetadata* meta = repository_.find(type);
            if (!meta)
                continue;

            int originX = entry["origin"]["x"];
            int originY = entry["origin"]["y"];

            int teleportMapId = -1;
            int teleportX = 0;
            int teleportY = 0;

            if (entry.contains("teleport_target"))
            {
                auto tt = entry["teleport_target"];
                teleportMapId = tt["map_id"];
                teleportX = tt["x"];
                teleportY = tt["y"];
            }

            applyFootprint(*map, *meta, originX, originY,
                            teleportMapId, teleportX, teleportY);


            auto entity = std::make_unique<Entity>();

            entity->add<PositionComponent>(originX, originY);
            entity->add<MapObjectRenderComponent>(
                meta,
                originX,
                originY,
                teleportMapId,
                teleportX,
                teleportY
            );

            // entity->add<PositionComponent>(
            //     originX,
            //     originY
            // );
            // entity.add<FreeMovementComponent>(
                
            // );

            map->addMapObject(std::move(entity));
        }
    }

    return map;
}