#include "system/CollisionSystem.h"
#include "component/CollisionComponent.h"

namespace CollisionSystem {

bool isAreaBlocked(Registry& registry, const AABB& box, EntityID self)
{
    for (EntityID id : registry.view<CollisionComponent, PositionComponent>()) {
        if (id == self)
            continue;

        auto* collision = registry.get<CollisionComponent>(id);
        if (!collision->solid)
            continue; // triggers (ground items, warps) never block movement

        auto* position = registry.get<PositionComponent>(id);

        AABB objBox = collision->resolve(position->x, position->y);
        if (box.intersects(objBox))
            return true;
    }
    return false;
}

}