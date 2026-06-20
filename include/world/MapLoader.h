#pragma once

#include <memory>
#include <string>
#include <vector>

#include "world/Map.h"
#include "world/MapObjectRepository.h"
#include "world/TileRepository.h"

struct MapMetadata {
    int id;
    std::string path;
};

// ---------------------------------------------------------------------------
// MapLoader — builds Map instances from JSON files on disk.
//
// Responsibility, unchanged from before, narrowed to just this:
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
        MapObjectRepository& repository,
        TileRepository& tileRepository
    );

    std::unique_ptr<Map> loadMapById(int mapId) const;

private:
    MapObjectRepository& repository_;
    TileRepository& tileRepo_;
    std::string maps_folder_;
    std::vector<MapMetadata> maps_metadata_;

    void loadMetadata(const std::string& mapsFolder);

    // Applies one placed object's footprint cells onto the map's tiles.
    // Each cell's traits (blocking, teleport) are applied independently —
    // adding a new trait kind means adding one branch here, not touching
    // the surrounding parsing/lookup code.
    void applyFootprint(Map& map, const ObjectTypeMetadata& meta,
                         int originX, int originY,
                         int teleportMapId, int teleportX, int teleportY) const;
};