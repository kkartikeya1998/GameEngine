#pragma once

#include <functional>

#include "engine/ecs/Registry.h"
#include "game/world/Map.h"
#include "game/ecs/movement/PositionComponent.h"
#include "game/ecs/movement/VelocityComponent.h"
#include "game/ecs/movement/DirectionComponent.h"
#include "game/ecs/movement/FreeMovementComponent.h"
#include "game/ecs/movement/MovementStateComponent.h"
#include "game/ecs/collision/CollisionComponent.h"
#include "game/ecs/player/PlayerControlComponent.h"
#include "game/ecs/movement/MovementCapabilityComponent.h"

namespace MovementSystem
{
    constexpr float kMaxFrameDt = 1.f / 20.f; // guards against tunneling through thin colliders on frame hitches

    void update(Registry &registry, EntityID id, const Map &map, float dt,
                const std::function<bool(const AABB &)> &isBlocked,
                const PlayerControlComponent *input = nullptr);

    bool mapBoundsCheck(const Map &map, const AABB &aabb);
}