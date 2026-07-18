#pragma once
#include <string>
#include "entities/Component.h"

// Marks an entity as a ground item pickup. Render/collision come from the
// entity's normal archetype — this component
// only carries what's specific to *this* placed instance.
struct WorldItemComponent : public Component {
    std::string itemId;
    int quantity = 1;

    WorldItemComponent() = default;
    WorldItemComponent(std::string itemId, int quantity)
        : itemId(std::move(itemId)), quantity(quantity) {}
};