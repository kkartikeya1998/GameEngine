#pragma once

#include "Terrain.h"
#include "world/IWalkable.h"
#include "world/IInteractable.h"
#include "world/ITeleportable.h"

// All small implementations and behaviour separated into triats.
// Doesn't need a separate definition file, but could be split into a .cpp if it grows too large.
class Tile {
public:
    explicit Tile(Terrain::Type type = Terrain::Type::Grass)
        : terrain_type(type) {}

    Terrain::Type terrain() const {
        return terrain_type;
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
 
    // Each trait is optional. nullptr = tile doesn't support this behavior.
    std::unique_ptr<IWalkable>     walkable_;
    std::unique_ptr<IInteractable> interactable_;
    std::unique_ptr<ITeleportable> teleportable_;
};