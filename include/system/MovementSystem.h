#pragma once
#include <functional>
#include "entities/Entity.h"
#include "tmp/movement/PositionComponent.h"
#include "tmp/movement/VelocityComponent.h"
#include "tmp/movement/DirectionComponent.h"
#include "tmp/movement/GridMovementComponent.h"
#include "tmp/movement/FreeMovementComponent.h"

namespace MovementSystem {

// Direction is now a separate parameter — position no longer carries facing.
void updateGrid(PositionComponent& position, DirectionComponent& direction,
                GridMovementComponent& movement,
                Direction inputDir);

AABB getGridHitbox(const PositionComponent& position);

// VelocityComponent is now persistent state written by this function,
// not a local dx/dy that dies each frame.
void updateFree(PositionComponent& position, VelocityComponent& velocity,
                DirectionComponent& direction, FreeMovementComponent& movement,
                float dt, Direction inputDir,
                const std::function<bool(const AABB&)>& isBlocked);

AABB getFreeHitbox(const PositionComponent& position, const FreeMovementComponent& movement);
AABB freeHitboxAt(const FreeMovementComponent& movement, float x, float y);

} // namespace MovementSystem