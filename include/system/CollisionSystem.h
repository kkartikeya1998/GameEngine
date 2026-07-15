#pragma once
#include "entities/Registry.h"
#include "component/PositionComponent.h" // for AABB

namespace CollisionSystem {
    // Checks box against every entity with both a CollisionComponent and a
    // PositionComponent, excluding `self` (so a moving entity doesn't
    // collide against its own not-yet-updated collision box).
    bool isAreaBlocked(Registry& registry, const AABB& box, EntityID self = NullEntity);
}