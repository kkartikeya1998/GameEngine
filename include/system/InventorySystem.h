#pragma once
#include "component/InventoryComponent.h"
#include "asset/AssetDatabase.h"
#include "events/Events.h"
#include "entities/Registry.h"

// Owns inventory mutation rules (stacking/capacity) and the ItemConsumed
// event handler. Does NOT own item behavior — that's InteractionManager's
// job via ItemMetadata::interactionId.
namespace InventorySystem {
    // Adds `quantity` of itemId, filling existing stacks up to
    // ItemMetadata::maxStack before opening a new stack. Returns the amount
    // that didn't fit (0 if everything was added) — caller decides what to
    // do with overflow (drop it, block pickup, etc).
    int addItem(InventoryComponent& inv, const AssetDatabase& assets,
                const std::string& itemId, int quantity);

    // Removes up to `quantity`, deleting emptied stacks. Returns amount
    // actually removed (may be less than requested).
    int removeItem(InventoryComponent& inv, const std::string& itemId, int quantity);

    bool hasItem(const InventoryComponent& inv, const std::string& itemId);
    int countItem(const InventoryComponent& inv, const std::string& itemId);

    // Wired into Game's EventQueue::Drain visitor alongside your other
    // handlers. Decrements the consumed item from its owner's inventory.
    void handleItemConsumed(Registry& registry, const ItemConsumed& e);
}