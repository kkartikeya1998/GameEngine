#include "world/World.h"

World::World(const AssetDatabase& assets)
    : loader_(assets)
{
}

void World::loadMap(int mapId)
{
    registry_.clear();
    loader_.loadInto(registry_, active_map_, mapId);
}