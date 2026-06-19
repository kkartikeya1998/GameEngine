#include "world/MapLoader.h"
#include "world/Terrain.h"
#include "world/Map.h"
#include "world/MapObject.h"
#include "world/MapObjectRepository.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "external/json.hpp"

using json = nlohmann::json;

MapLoader::MapLoader(
    const std::string& mapsFolder,
    MapObjectRepository& repository
)
    : repository_(repository), maps_folder_(mapsFolder)
{
    loadObjectMetadata();
    loadMetadata(mapsFolder);
}

void MapLoader::loadObjectMetadata()
{
    namespace fs = std::filesystem;

    // Expect object_metadata.json in the maps folder
    fs::path objectMetaPath = fs::path(maps_folder_) / "object_metadata.json";

    if (!fs::exists(objectMetaPath)) {
        std::cerr << "object_metadata.json not found at: " << objectMetaPath << '\n';
        return;
    }

    std::ifstream file(objectMetaPath);
    if (!file) {
        std::cerr << "Failed to open object metadata file: " << objectMetaPath << '\n';
        return;
    }

    json j;
    file >> j;

    const auto& object_types = j.at("object_types");
    for (auto it = object_types.begin(); it != object_types.end(); ++it) {
        const std::string& typeName = it.key();
        const json& def = it.value();

        ObjectTypeMetadata meta;
        meta.texturePath = def.value("texture", "");

        for (const auto& cell : def.at("footprint")) {
            FootprintCell fc;
            fc.dx = cell.value("dx", 0);
            fc.dy = cell.value("dy", 0);
            fc.blocking = cell.value("blocking", false);
            fc.isTeleportTile = cell.value("teleport", false);
            meta.footprint.push_back(fc);
        }

        repository_.register_type(typeName, std::move(meta));
    }
}

void MapLoader::loadMetadata(const std::string& mapsFolder)
{
    namespace fs = std::filesystem;

    maps_metadata_.clear();

    for (const auto& entry : fs::directory_iterator(mapsFolder))
    {
        if (entry.path().extension() != ".json")
            continue;

        // Skip object_metadata.json
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

    // tiles
    const auto& tiles_json = j["tiles"];
    
    std::cout << tiles_json.size() << " tiles, expected " << (width * height) << '\n';
    if (tiles_json.size() != static_cast<std::size_t>(width * height)) {
        throw std::runtime_error("Tile count does not match width * height");
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int flat = y * width + x;

            map->set_tile(
                x,
                y,
                Terrain::terrain_from_string(
                    tiles_json[flat].get<std::string>()
                )
            );
        }
    }

    // objects
    if (j.contains("map_objects"))
    {
        for (const auto& entry : j["map_objects"])
        {
            std::string type = entry["type"];

            const ObjectTypeMetadata* meta =
                repository_.find(type);

            if (!meta)
                continue;

            int originX = entry["origin"]["x"];
            int originY = entry["origin"]["y"];

            int teleportMapId = -1;
            int teleportX = 0;
            int teleportY = 0;

            // if (entry.contains("teleport_target"))
            // {
            //     auto tt = entry["teleport_target"];

            //     teleportMapId = tt["map_id"];
            //     teleportX = tt["x"];
            //     teleportY = tt["y"];
            // }

            map->addMapObject(
                std::make_unique<MapObject>(
                    meta,
                    originX,
                    originY,
                    teleportMapId,
                    teleportX,
                    teleportY
                )
            );
        }
    }

    return map;
}
