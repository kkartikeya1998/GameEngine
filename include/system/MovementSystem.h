#pragma once

#include <functional>

#include "entities/Entity.h"
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

    // velocity is now genuine units/second; dt is applied once here.
    void update(Entity &entity, const PlayerControlComponent &input, const Map &map,
                float dt, const std::function<bool(const AABB &)> &isBlocked);

    bool mapBoundsCheck(const Map &map, const AABB &aabb);

} // namespace MovementSystem