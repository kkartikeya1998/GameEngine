#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Map.h"

struct MapMetadata {
    int id;
    std::string path;
};

class World {
private:
    std::vector<MapMetadata> maps_metadata;
    std::unique_ptr<Map> active_map;

public:
    World() = delete;
    explicit World(const std::string& world_file);

    Map* getActiveMap() const;
        
    void load_metadata(const std::string& world_file);
    void load_map(int map_id);
    // void render() const;
};