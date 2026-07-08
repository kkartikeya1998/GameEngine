#pragma once

#include <functional>

#include "entities/Entity.h"
#include "world/Map.h"
#include "tmp/component/PositionComponent.h"
#include "tmp/component/VelocityComponent.h"
#include "tmp/component/DirectionComponent.h"
#include "tmp/component/FreeMovementComponent.h"
#include "tmp/component/CollisionComponent.h"
#include "tmp/component/MovementStateComponent.h"
#include "tmp/component/PlayerControlComponent.h"

namespace MovementSystem
{

    // velocity is now genuine units/second; dt is applied once here.
    void update(Entity &entity, const PlayerControlComponent &input, const Map &map,
                float dt, const std::function<bool(const AABB &)> &isBlocked);

    bool mapBoundsCheck(const Map &map, const AABB &aabb);

} // namespace MovementSystem