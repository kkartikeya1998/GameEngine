#pragma once

#include <functional>

#include "entities/Entity.h"
#include "tmp/movement/PositionComponent.h"
#include "tmp/movement/VelocityComponent.h"
#include "tmp/movement/DirectionComponent.h"
#include "tmp/movement/FreeMovementComponent.h"
#include "tmp/movement/CollisionComponent.h"
#include "tmp/movement/MovementStateComponent.h"

namespace MovementSystem {

// velocity is now genuine units/second; dt is applied once here.
void updateFree(PositionComponent& position, VelocityComponent& velocity,
                DirectionComponent& direction, FreeMovementComponent& movement,
                CollisionComponent& collision, MovementStateComponent& state,
                float dt, Direction inputDir, bool sprintRequested,
                const std::function<bool(const AABB&)>& isBlocked);

} // namespace MovementSystem