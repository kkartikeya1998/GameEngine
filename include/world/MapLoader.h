#pragma once

#include <memory>
#include <string>
#include <vector>

#include "world/Map.h"
#include "entities/Entity.h"
#include "asset/AssetDatabase.h"

struct MapMetadata {
    int id;
    std::string path;
};

class MapLoader {
public:
    explicit MapLoader(const AssetDatabase& assets);

    std::unique_ptr<Map> loadMapById(int mapId) const;

private:
    const AssetDatabase& assets_;
    std::string maps_folder_;
    std::vector<MapMetadata> maps_metadata_;

    void loadMetadata();
};