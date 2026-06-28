#pragma once

#include "entities/player/Player.h"
// ---------------------------------------------------------------------------
// IInteractable — capability interface for tiles the player can interact
// with (sign posts, NPCs standing on a tile, item pickups, doors).
//
// Empty implementation for now — interact() is a no-op until dialogue/
// item systems exist. The interface is the contract; behavior comes later.
// ---------------------------------------------------------------------------
class IInteractable {
public:
    virtual ~IInteractable() = default;
    virtual void interact(Entity& entity) = 0;
};