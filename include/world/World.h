#pragma once

#include <memory>

#include "world/Map.h"
#include "world/MapLoader.h"
#include "world/MapObjectRepository.h"

// ---------------------------------------------------------------------------
// World — owns the active Map and the MapLoader that builds it.
// ---------------------------------------------------------------------------
class World {
public:
    World(const std::string& mapsFolder, MapObjectRepository& objectRepository);

    Map* getActiveMap() const;

    void loadMap(int mapId);

private:
    MapLoader loader_;

    std::unique_ptr<Map> active_map_;
};