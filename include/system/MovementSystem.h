#pragma once

#include <functional>

#include "entities/Entity.h"
#include "tmp/component/PositionComponent.h"
#include "tmp/component/VelocityComponent.h"
#include "tmp/component/DirectionComponent.h"
#include "tmp/component/FreeMovementComponent.h"
#include "tmp/component/CollisionComponent.h"
#include "tmp/component/MovementStateComponent.h"

namespace MovementSystem {

// velocity is now genuine units/second; dt is applied once here.
void update(PositionComponent& position, VelocityComponent& velocity,
                DirectionComponent& direction, FreeMovementComponent& movement,
                CollisionComponent& collision, MovementStateComponent& state,
                float dt, Direction inputDir, bool sprintRequested,
                const std::function<bool(const AABB&)>& isBlocked);

} // namespace MovementSystem