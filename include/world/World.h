#pragma once

#include <memory>
#include <vector>

#include "world/Map.h"
#include "world/MapLoader.h"
#include "asset/AssetDatabase.h"
#include "entities/Entity.h"

class World
{
public:
    explicit World(const AssetDatabase& assets);

    const Map *getActiveMap() const;
    void loadMap(int mapId);

    std::vector<Entity *> getAllEntities() const;

    template <typename T>
    std::vector<Entity *> view() const
    {
        std::vector<Entity *> result;
        for (Entity *e : getAllEntities())
            if (e->has<T>())
                result.push_back(e);
        return result;
    }

private:
    MapLoader loader_;
    std::unique_ptr<Map> active_map_;
};