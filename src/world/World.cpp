#include "world/World.h"

World::World(const std::string& mapsFolder)
    : repository_(),
      tileRepository_(mapsFolder + "/tileset_metadata.json"),
      loader_(mapsFolder, repository_, tileRepository_)
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