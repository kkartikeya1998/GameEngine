#pragma once

#include <functional>

#include "engine/ecs/Registry.h"
#include "game/world/Map.h"
#include "game/ecs/actor/PositionComponent.h"
#include "game/ecs/actor/VelocityComponent.h"
#include "game/ecs/actor/DirectionComponent.h"
#include "game/ecs/actor/FreeMovementComponent.h"
#include "game/ecs/actor/CollisionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/PlayerControlComponent.h"
#include "game/ecs/actor/MovementCapabilityComponent.h"

namespace MovementSystem
{
    constexpr float kMaxFrameDt = 1.f / 20.f; // guards against tunneling through thin colliders on frame hitches

    void update(Registry &registry, EntityID id, const Map &map, float dt,
                const std::function<bool(const AABB &)> &isBlocked,
                const PlayerControlComponent *input = nullptr);

    bool mapBoundsCheck(const Map &map, const AABB &aabb);
}