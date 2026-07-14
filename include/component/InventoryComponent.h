#pragma once
#include <string>
#include <vector>
#include "entities/Component.h"

// One stack of a single item type — id for lookup, name for display, quantity to track count
struct ItemStack {
    std::string itemId;
    std::string displayName;
    int quantity = 0;
};

// Player-carried items — pure data, mutated by inventory action commands
struct InventoryComponent : public Component {
    std::vector<ItemStack> items;
};