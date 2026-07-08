#include "world/World.h"

World::World(MapObjectRepository& objectRepository, TileRepository& tileRepository)
    : loader_(objectRepository, tileRepository)
{
}

void World::loadMap(int mapId)
{
    active_map_ = loader_.loadMapById(mapId);
}

const Map* World::getActiveMap() const
{
    return active_map_.get();
}

std::vector<Entity*> World::getAllEntities() const
{
    std::vector<Entity*> result;

    if (!active_map_)
        return result;

    const auto& objects = active_map_->getMapObjects();
    const auto& npcs = active_map_->getNpcs();

    result.reserve(objects.size() + npcs.size());

    for (const auto& obj : objects)
        result.push_back(obj.get());

    for (const auto& npc : npcs)
        result.push_back(npc.get());

    return result;
}