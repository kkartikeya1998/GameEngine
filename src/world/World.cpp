#include "world/World.h"

World::World(const std::string& mapsFolder, MapObjectRepository& objectRepository)
    : loader_(mapsFolder, objectRepository)
{
}

void World::loadMap(int mapId)
{
    active_map_ = loader_.loadMapById(mapId);
}

Map* World::getActiveMap() const
{
    return active_map_.get();
}