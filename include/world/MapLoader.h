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

class MapLoader {
public:
    MapLoader(const std::string& mapsFolder, MapObjectRepository& repository);

    std::unique_ptr<Map> load(int mapId) const;

private:
    void loadObjectMetadata();
    void loadMetadata(const std::string& mapsFolder);

    MapObjectRepository& repository_;
    std::string maps_folder_;
    std::vector<MapMetadata> maps_metadata_;
};