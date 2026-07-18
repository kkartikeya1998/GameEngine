#pragma once
#include <string>
#include <vector>
#include "entities/Component.h"

// One stack of a single item type. No displayName — resolved from
// ItemMetadata via itemId at query/display time, so renaming an item
// in JSON doesn't require touching every inventory that holds it.
struct ItemStack {
    std::string itemId;
    int quantity = 0;
};

struct InventoryComponent : public Component {
    std::vector<ItemStack> items;
};