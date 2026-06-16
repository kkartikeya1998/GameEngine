#include <filesystem>
#include <fstream>
#include <iostream>

#include "world/World.h"
#include "external/json.hpp"

using json = nlohmann::json;

World::World(const std::string& world_file) {
    load_metadata(world_file);
}

void World::load_metadata(const std::string& maps_folder)
{
    maps_metadata.clear();

    namespace fs = std::filesystem;

    for (const auto& entry : fs::directory_iterator(maps_folder))
    {
        if (entry.path().extension() != ".json")
            continue;

        std::ifstream file(entry.path());
        if (!file)
            continue;

        json j;
        file >> j;

        int id = j.value("id", -1);
        std::string path = entry.path().string();

        maps_metadata.emplace_back(MapMetadata{ id, path });

        auto& last_map = maps_metadata[maps_metadata.size() - 1];
        std::cout << "Loaded map metadata: " << last_map.id << " from " << last_map.path << '\n';
    
    }
    std::cout << "Total maps loaded: " << maps_metadata.size() << '\n';
}
void World::load_map(int map_id)
{
    for (const auto& meta : maps_metadata)
    {
        if (meta.id == map_id)
        {
            active_map = std::make_unique<Map>(meta.path);
            return;
        }
    }

    std::cerr << "Map not found: " << map_id << '\n';
    active_map.reset();
}

void World::render() const {
    if (active_map) {
        active_map->render();
    }
}