#pragma once

#include <memory>
#include <string>
#include <vector>

#include "world/Map.h"
#include "entities/Entity.h"
#include "asset/repositories/MapObjectRepository.h"
#include "asset/repositories/TileRepository.h"

struct MapMetadata {
    int id;
    std::string path;
};

// ---------------------------------------------------------------------------
// MapLoader — builds Map instances from JSON files on disk.
// ---------------------------------------------------------------------------
class MapLoader {
public:
    MapLoader(
        MapObjectRepository& mapObjectRepository,
        TileRepository& tileRepository
    );

    std::unique_ptr<Map> loadMapById(int mapId) const;

private:
    MapObjectRepository& mapObjectRepository_;
    TileRepository& tileRepository_;
    std::string maps_folder_;
    std::vector<MapMetadata> maps_metadata_;

    void loadMetadata();

    // std::unique_ptr<Entity> buildNpc(const std::string& typeName,
    //                                    int gridX, int gridY) const;
};