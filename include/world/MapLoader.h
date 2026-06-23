#pragma once

#include <memory>
#include <string>
#include <vector>

#include "world/Map.h"
#include "world/MapObjectRepository.h"

struct MapMetadata {
    int id;
    std::string path;
};

// ---------------------------------------------------------------------------
// MapLoader — builds Map instances from JSON files on disk.
//
// Responsibilities:
//   - locate map files in maps_folder_
//   - parse a chosen map's JSON into tiles + placed objects
//   - apply each placed object's footprint traits onto the resulting tiles
//
// Object-type metadata loading (object_metadata.json) now lives entirely
// in MapObjectRepository — MapLoader only triggers it once at construction
// and never re-implements the parsing itself.
// ---------------------------------------------------------------------------
class MapLoader {
public:
    MapLoader(
        const std::string& mapsFolder,
        MapObjectRepository& repository
    );

    std::unique_ptr<Map> loadMapById(int mapId) const;

private:
    MapObjectRepository& repository_;
    std::string maps_folder_;
    std::vector<MapMetadata> maps_metadata_;

    void loadMetadata(const std::string& mapsFolder);

    void applyFootprint(Map& map, const ObjectTypeMetadata& meta,
                         int originX, int originY,
                         int teleportMapId, int teleportX, int teleportY) const;
};