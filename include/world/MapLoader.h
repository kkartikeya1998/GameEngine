#pragma once

#include <memory>
#include <string>
#include <vector>

#include "world/Map.h"
#include "entities/Entity.h"
#include "asset/repositories/MapObjectRepository.h"

struct MapMetadata {
    int id;
    std::string path;
};

// ---------------------------------------------------------------------------
// MapLoader — builds Map instances from JSON files on disk.
// ---------------------------------------------------------------------------
class MapLoader {
public:
    MapLoader(
        MapObjectRepository& repository
    );

    std::unique_ptr<Map> loadMapById(int mapId) const;

private:
    MapObjectRepository& repository_;
    std::string maps_folder_;
    std::vector<MapMetadata> maps_metadata_;

    void loadMetadata();

    // void applyFootprint(Map& map, const ObjectTypeMetadata& meta,
    //                      int originX, int originY) const;

    // UNIMPLEMENTED — see class comment above. Would need to build an
    // Entity carrying a movement component (GridMovementComponent for
    // gridX/gridY), a matching render component (GridRenderComponent),
    // and an NpcComponent{typeName}, then return it for
    // loadMapById to pass to map->addNpc(...).
    std::unique_ptr<Entity> buildNpc(const std::string& typeName,
                                       int gridX, int gridY) const;
};