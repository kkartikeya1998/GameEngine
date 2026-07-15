#pragma once

#include <functional>

#include "entities/Registry.h"
#include "world/Map.h"
#include "component/PositionComponent.h"
#include "component/VelocityComponent.h"
#include "component/DirectionComponent.h"
#include "component/FreeMovementComponent.h"
#include "component/CollisionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/PlayerControlComponent.h"

namespace MovementSystem
{
    // input == nullptr: velocity/direction are used as already set by the
    // caller (e.g. CreatureAISystem) rather than derived from input.
    void update(Registry &registry, EntityID id, const Map &map, float dt,
                const std::function<bool(const AABB &)> &isBlocked,
                const PlayerControlComponent *input = nullptr);

    bool mapBoundsCheck(const Map &map, const AABB &aabb);
}