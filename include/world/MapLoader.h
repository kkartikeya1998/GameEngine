#pragma once

#include <string>
#include <vector>

#include "world/Map.h"
#include "entities/Registry.h"
#include "asset/AssetDatabase.h"

struct MapMetadata {
    int id;
    std::string path;
};

class MapLoader {
public:
    explicit MapLoader(const AssetDatabase& assets);

    // Clears and repopulates `map` with tile data, and spawns map objects
    // + wild pokemon into `registry`. Caller (World) is responsible for
    // calling registry.clear() beforehand if a previous map's entities
    // need to go away first.
    void loadInto(Registry& registry, Map& map, int mapId) const;

private:
    const AssetDatabase& assets_;
    std::string maps_folder_;
    std::vector<MapMetadata> maps_metadata_;

    void loadMetadata();
};