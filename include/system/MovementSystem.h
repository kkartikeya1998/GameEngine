#pragma once

#include <functional>

#include "entities/Entity.h"
#include "tmp/movement/PositionComponent.h"
#include "tmp/movement/VelocityComponent.h"
#include "tmp/movement/DirectionComponent.h"
#include "tmp/movement/FreeMovementComponent.h"

namespace MovementSystem {

void updateFree(PositionComponent& position, VelocityComponent& velocity,
                DirectionComponent& direction, FreeMovementComponent& movement,
                float dt, Direction inputDir,
                const std::function<bool(const AABB&)>& isBlocked);

AABB getFreeHitbox(const PositionComponent& position, const FreeMovementComponent& movement);
AABB freeHitboxAt(const FreeMovementComponent& movement, float x, float y);

} // namespace MovementSystem