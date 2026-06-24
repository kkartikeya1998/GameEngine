#pragma once

#include <string>

#include "Terrain.h"
#include "world/IWalkable.h"
#include "world/IInteractable.h"
#include "world/ITeleportable.h"

// All small implementations and behaviour separated into triats.
// Doesn't need a separate definition file, but could be split into a .cpp if it grows too large.
//
// CHANGED: Tile now stores typeName alongside terrain_type — the original
// JSON catalog key (e.g. "Grass2", "Sand") that MapLoader previously
// converted to Terrain::Type and discarded. The two fields serve different
// systems and are kept deliberately separate rather than one replacing
// the other:
//
//   - terrain_type (Terrain::Type) drives GAMEPLAY defaults — currently
//     only isWalkable()'s fallback via Terrain::isWalkableBase(). This
//     stays an enum because gameplay code wants to reason about broad
//     terrain categories (is this water, is this generally walkable),
//     not specific art variants.
//
//   - typeName (std::string) drives RENDERING — it's the exact catalog
//     key TileRepository indexes by. This lets two tiles share the same
//     Terrain::Type (same walkability rules) while looking completely
//     different (e.g. "Grass1" vs a future "GrassAutumn" could both be
//     Terrain::Type::Grass1 for collision purposes, but render different
//     art) — something the old enum-only design couldn't express, since
//     TileAtlas had to hardcode a single enum->name mapping.
//
// Defaults typeName to terrain_type's own toString() so a Tile constructed
// the old way (type only, no explicit name) still renders something
// sensible rather than an empty lookup key.
class Tile {
public:
    explicit Tile(Terrain::Type type = Terrain::Type::Grass1)
        : terrain_type(type), type_name(std::string(Terrain::toString(type))) {}

    Tile(Terrain::Type type, std::string typeName)
        : terrain_type(type), type_name(std::move(typeName)) {}

    Terrain::Type terrain() const {
        return terrain_type;
    }

    // The catalog key TileRepository should be queried with — what the
    // renderer actually uses now instead of terrain().
    const std::string& typeName() const {
        return type_name;
    }

    char render_char() const {
        return Terrain::toString(terrain_type)[0];
    }

      // -----------------------------------------------------------------------
    // Walkability — terrain base, overridden if an IWalkable trait is set.
    // -----------------------------------------------------------------------
    bool isWalkable() const {
        if (walkable_) return walkable_->canWalk();
        return Terrain::isWalkableBase(terrain_type);
    }
    void setWalkable(std::unique_ptr<IWalkable> trait) {
        walkable_ = std::move(trait);
    }
 
    // -----------------------------------------------------------------------
    // Interactable — empty/no-op until dialogue or item systems exist.
    // -----------------------------------------------------------------------
    bool isInteractable() const { return interactable_ != nullptr; }
    IInteractable* getInteractable() const { return interactable_.get(); }
    void setInteractable(std::unique_ptr<IInteractable> trait) {
        interactable_ = std::move(trait);
    }
 
    // -----------------------------------------------------------------------
    // Teleportable
    // -----------------------------------------------------------------------
    bool isTeleportable() const { return teleportable_ != nullptr; }
    ITeleportable* getTeleportable() const { return teleportable_.get(); }
    void setTeleportable(std::unique_ptr<ITeleportable> trait) {
        teleportable_ = std::move(trait);
    }
 
private:
    Terrain::Type terrain_type;
    std::string type_name;
 
    // Each trait is optional. nullptr = tile doesn't support this behavior.
    std::unique_ptr<IWalkable>     walkable_;
    std::unique_ptr<IInteractable> interactable_;
    std::unique_ptr<ITeleportable> teleportable_;
};