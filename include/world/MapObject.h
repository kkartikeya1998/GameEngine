#pragma once
#include <string>
#include <vector>

// A relative offset from the object's origin tile, plus what role
// that specific cell plays for walkability/teleport/interact purposes.
struct FootprintCell {
    int dx, dy;              // offset from origin (0,0 = origin tile itself)
    bool blocking;           // true = this cell blocks movement
    bool isTeleportTile;     // true = this cell is the teleport entry point
    bool isInteractTile;     // true = this cell triggers interaction (sign, NPC)
};
// ---------------------------------------------------------------------------
// ObjectTypeMetadata — reusable definition of one object TYPE.
//
// Shared by every placed instance of that type across every map. Holds the
// texture path and footprint shape — never an origin position, since a type
// has no position; only a placed instance does.
// ---------------------------------------------------------------------------

struct ObjectTypeMetadata {
    std::string texturePath;
    std::vector<FootprintCell> footprint;
};
// ---------------------------------------------------------------------------
// MapObject
//
// Represents ONE placed object instance on a map.
//
// Stores:
//   - shared metadata pointer (non-owning, repository owns metadata)
//   - origin tile position
//   - optional teleport destination
//
// Does NOT own texture or footprint definitions.
// ---------------------------------------------------------------------------
class MapObject {
public:
    MapObject(
        const ObjectTypeMetadata* metadata,
        int originX,
        int originY,
        int teleportTargetMapId = -1,
        int teleportTargetX = 0,
        int teleportTargetY = 0
    );

    // -------------------------
    // Metadata access
    // -------------------------
    const std::string& getTexturePath() const;
    const std::vector<FootprintCell>& getFootprint() const;

    // -------------------------
    // Position
    // -------------------------
    int getOriginX() const;
    int getOriginY() const;

    // -------------------------
    // Teleport data
    // -------------------------
    int getTeleportTargetMapId() const;

    void getTeleportTarget(int& x, int& y) const;

private:
    // Shared metadata (repository owns this)
    const ObjectTypeMetadata* metadata_;

    // Instance-specific data
    int originX_;
    int originY_;

    int teleportTargetMapId_;
    int teleportTargetX_;
    int teleportTargetY_;
};