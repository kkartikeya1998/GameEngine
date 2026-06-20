#pragma once

#include <memory>

#include "world/Map.h"
#include "world/MapLoader.h"
#include "world/MapObjectRepository.h"
#include "world/TileRepository.h"

class World {
public:
    explicit World(const std::string& mapsFolder);

    Map* getActiveMap() const;

    void loadMap(int mapId);

private:
    MapObjectRepository repository_;
    TileRepository      tileRepository_;
    MapLoader           loader_;

    std::unique_ptr<Map> active_map_;
};