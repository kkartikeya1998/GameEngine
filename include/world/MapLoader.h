#pragma once

#include <memory>
#include <string>
#include <vector>

#include "world/Map.h"
#include "entities/Entity.h"
#include "asset/MapObjectRepository.h"

struct MapMetadata {
    int id;
    std::string path;
};

// ---------------------------------------------------------------------------
// MapLoader — builds Map instances from JSON files on disk.
//
// CHANGED (ECS pass): GridMovementMechanics, FreeMovementMechanics, and
// AnimationComponentAdapter (referenced in this header's older comment
// block, now stale) no longer exist as classes — see
// system/MovementSystem.h and system/RenderStateSystem.h, where their
// logic moved to. buildNpc's return type changes from
// std::unique_ptr<Npc> (Npc is also gone — see entities/npc/
// NpcComponent.h) to std::unique_ptr<Entity>.
//
// IMPORTANT: buildNpc has NO DEFINITION in the .cpp I was given — it's
// declared here but was never implemented or called anywhere in the
// loadMapById body shown to me (no "npcs" JSON key is read there
// either). This is NOT something I ported, because there was no body
// to port. The declaration below is updated only so this header
// compiles against the new Entity-based world; if you actually want
// NPCs loaded from map JSON, this function's BODY still needs to be
// written from scratch — see the comment on its declaration below for
// what that body would need to build.
// ---------------------------------------------------------------------------
class MapLoader {
public:
    MapLoader(
        const std::string& mapsFolder,
        MapObjectRepository& repository
    );

    std::unique_ptr<Map> loadMapById(int mapId) const;

private:
    MapObjectRepository& repository_;
    std::string maps_folder_;
    std::vector<MapMetadata> maps_metadata_;

    void loadMetadata(const std::string& mapsFolder);

    void applyFootprint(Map& map, const ObjectTypeMetadata& meta,
                         int originX, int originY,
                         int teleportMapId, int teleportX, int teleportY) const;

    // UNIMPLEMENTED — see class comment above. Would need to build an
    // Entity carrying a movement component (GridMovementComponent for
    // gridX/gridY), a matching render component (GridRenderComponent),
    // and an NpcComponent{typeName}, then return it for
    // loadMapById to pass to map->addNpc(...).
    std::unique_ptr<Entity> buildNpc(const std::string& typeName,
                                       int gridX, int gridY) const;
};