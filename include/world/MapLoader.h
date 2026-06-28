#pragma once

#include <memory>
#include <string>
#include <vector>

#include "world/Map.h"
#include "asset/MapObjectRepository.h"

struct MapMetadata {
    int id;
    std::string path;
};

// ---------------------------------------------------------------------------
// MapLoader — builds Map instances from JSON files on disk.
//
// CHANGED (this pass): buildNpc's TODO ("GridMovementMechanics
// constructor signature unconfirmed") is resolved — GridMovementMechanics
// still takes (x, y, dir), unchanged signature, see
// GridMovementMechanics.h. What changed is what buildNpc constructs
// AROUND it: instead of producing an Entity subclass instance, it now
// assembles an Entity directly from a GridMovementMechanics +
// AnimationComponentAdapter pair (the grid-movement/grid-render
// combination, matching "the exact choice GridEntity used to hardcode"
// — see AnimationComponentAdapter.h), wrapped in an Npc.
//
// Movement-family selection still lives here today as a single fixed
// choice (grid movement, AnimationComponentAdapter render) — not yet
// branching on any "movement" JSON field. When that's needed, this is
// still the one function that grows the branch; Map/RenderSystem/
// GameController do not change — same design intent as before, now
// just composing two components instead of picking a subclass.
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

    std::unique_ptr<Npc> buildNpc(const std::string& typeName,
                                   int gridX, int gridY) const;
};