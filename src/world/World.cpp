#include "world/World.h"

World::World(const std::string& mapsFolder)
    : repository_(mapsFolder + "/object_metadata.json"),
      loader_(mapsFolder, repository_)
{
}

void World::loadMap(int mapId)
{
    active_map_ = loader_.load(mapId);
}

Map* World::getActiveMap() const
{
    return active_map_.get();
}