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

// CHANGED: MapLoader no longer triggers MapObjectRepository's load. The
// repository now loads itself in its own constructor (called once by
// AssetManager at startup) — MapLoader receives it already populated and
// only ever reads from it via find(). This removes a responsibility
// MapLoader never should have had: deciding when object metadata loads.
// The old call here (repository_.load_from_file(...)) would no longer even
// compile, since load_from_file is now a private implementation detail of
// MapObjectRepository's constructor.
MapLoader::MapLoader(
    const std::string& mapsFolder,
    MapObjectRepository& repository
) : repository_(repository),
    maps_folder_(mapsFolder)
{
    loadMetadata(mapsFolder);
}

void MapLoader::loadMetadata(const std::string& mapsFolder)
{
    namespace fs = std::filesystem;

    maps_metadata_.clear();

    for (const auto& entry : fs::directory_iterator(mapsFolder))
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

    // originX/originY arrive in PIXEL space (the object's anchor point —
    // see MapObject's class comment / SFMLRenderer::drawMapObject).
    // Footprint cells (fc.dx/fc.dy) are tile-offsets and tile_at() takes
    // tile indices, so the anchor's containing tile is resolved here,
    // once, before any cell math — std::floor (not truncation) so
    // negative pixel coordinates round toward the correct tile, same
    // reasoning as the gridToPixel boundary-check fix elsewhere.
    int originTileX = static_cast<int>(std::floor(originX / GameConstants::TILE_SIZE));
    int originTileY = static_cast<int>(std::floor(originY / GameConstants::TILE_SIZE));

    // An object with a precise collisionBox (see CollisionBox in
    // MapObjectRepository.h) handles its own blocking via
    // Map::isAreaBlocked's separate object-AABB check — it does NOT
    // also get whole-tile blocking from its footprint here, or the
    // whole tile would be blocked anyway, making the precise box
    // pointless. 
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

            // CHANGED (ECS pass): no more std::make_unique<MapObject>.
            // MapObject is gone as a class — a MapObject "is" now just
            // an Entity carrying a MapObjectRenderComponent with
            // exactly the same fields MapObject's constructor used to
            // take. See entities/MapObjectRenderComponent.h and
            // system/MapObjectSystem.h for where its old methods moved
            // to.
            // CHANGED (Component base pass): MapObjectRenderComponent
            // is no longer an aggregate (Component's virtual
            // destructor disqualifies it — confirmed by compiling,
            // not assumed). add<T>(Args&&...) already forwards to
            // make_unique<T>(args...), so this becomes a normal
            // constructor call instead of brace-init; same six values,
            // same order, see MapObjectRenderComponent.h's added
            // constructor.
            auto entity = std::make_unique<Entity>();
            entity->add<MapObjectRenderComponent>(
                meta,
                originX,
                originY,
                teleportMapId,
                teleportX,
                teleportY
            );

            map->addMapObject(std::move(entity));
        }
    }

    return map;
}