#pragma once

#include "game/world/Map.h"
#include "game/world/MapLoader.h"
#include "engine/assets/AssetDatabase.h"
#include "engine/ecs/Registry.h"

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