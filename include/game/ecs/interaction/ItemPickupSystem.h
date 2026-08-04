#pragma once
#include "engine/ecs/Registry.h"
#include "engine/assets/AssetDatabase.h"
#include "engine/events/EventQueue.h"
#include "game/events/GameEvents.h"

namespace ItemPickupSystem {
    // Adds the world item's contents to `player`'s inventory, pushes
    // ItemPickedUp, and destroys `worldItem`. Caller is responsible for
    // deciding *when* this fires (overlap, interact-key, etc) — this
    // function only performs the mutation once that decision is made.
    void pickup(Registry& registry, const AssetDatabase& assets, EventQueue<GameEvent>& events,
                EntityID player, EntityID worldItem);
}