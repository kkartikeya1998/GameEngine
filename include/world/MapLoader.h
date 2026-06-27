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
// Responsibilities:
//   - locate map files in maps_folder_
//   - parse a chosen map's JSON into tiles + placed objects
//   - apply each placed object's footprint traits onto the resulting tiles
//   - NEW: parse a chosen map's "npcs" array (if present) into Npc instances
//
// Object-type metadata loading (object_metadata.json) now lives entirely
// in MapObjectRepository — MapLoader only triggers it once at construction
// and never re-implements the parsing itself.
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

    // NEW — sibling helper to applyFootprint, same role one level up:
    // takes one parsed "npcs" JSON entry's fields and produces an owned
    // Npc, ready for map.addNpc(). Kept private and file-scoped to
    // MapLoader, same as applyFootprint, since "JSON field names" should
    // never leak past this class.
    //
    // Movement-family selection lives here today as a single fixed
    // choice (GridEntity) — not yet branching on any "movement" JSON
    // field. When that's needed, this is the one function that grows
    // the branch; Map/RenderSystem/GameController do not change.
    std::unique_ptr<Npc> buildNpc(const std::string& typeName,
                                   int gridX, int gridY) const;
};