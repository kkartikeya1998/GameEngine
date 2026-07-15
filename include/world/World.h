#pragma once

#include "world/Map.h"
#include "world/MapLoader.h"
#include "asset/AssetDatabase.h"
#include "entities/Registry.h"

class World
{
public:
    explicit World(const AssetDatabase& assets);

    const Map& getActiveMap() const { return active_map_; }
    Registry& registry() { return registry_; }
    const Registry& registry() const { return registry_; }

    void loadMap(int mapId);

private:
    MapLoader loader_;
    Registry registry_;
    Map active_map_{1, 1}; // placeholder until first loadMap(); MapLoader replaces contents
};