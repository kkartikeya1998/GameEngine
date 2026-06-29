#include "world/World.h"

World::World(MapObjectRepository& objectRepository)
    : loader_(objectRepository)
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

std::vector<Entity*> World::getAllEntities() const
{
    std::vector<Entity*> result;

    if (!active_map_)
        return result;

    const auto& objects = active_map_->getMapObjects();
    const auto& npcs = active_map_->getNpcs();

    result.reserve(objects.size() + npcs.size());

    // Same const-unique_ptr-still-gives-mutable-pointee pattern
    // RenderSystem.cpp already relies on today (getNpcs()/
    // getMapObjects() are const accessors returning const
    // vector<unique_ptr<Entity>>&, but unique_ptr's operator->() on a
    // const unique_ptr still yields a non-const Entity*) — callers
    // get mutable Entity* here for the same reason RenderStateSystem's
    // stepGridRender/stepFreeRender calls already compile against
    // npcEntity->get<T>() today.
    for (const auto& obj : objects)
        result.push_back(obj.get());

    for (const auto& npc : npcs)
        result.push_back(npc.get());

    return result;
}