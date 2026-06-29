#pragma once

#include <functional>

#include "entities/Entity.h"
#include "entities/movement/PositionComponent.h"
#include "entities/movement/GridMovementComponent.h"
#include "entities/movement/FreeMovementComponent.h"

// ---------------------------------------------------------------------------
// MovementSystem — free functions holding the logic that used to live
// inside GridMovementMechanics::update() and FreeMovementMechanics::
// update(). Those classes are GONE; this is where their bodies moved
// to, ported field-for-field onto GridMovementComponent/
// FreeMovementComponent.
//
// CHANGED (PositionComponent pass): every function here now takes a
// PositionComponent& as an additional parameter — position used to be
// inlined on GridMovementComponent (gridX/gridY/facing) and
// FreeMovementComponent (x/y/facing); both now hold a sibling
// PositionComponent on the same Entity instead (see
// PositionComponent.h). Callers fetch both components before calling:
//
//   auto* pos = entity.get<PositionComponent>();
//   if (auto* g = entity.get<GridMovementComponent>())
//       MovementSystem::updateGrid(*pos, *g, inputDir, isBlocked);
//   else if (auto* f = entity.get<FreeMovementComponent>())
//       MovementSystem::updateFree(*pos, *f, dt, inputDir, isBlocked);
//
// Per the "collision check belongs in the mechanic" principle (kept
// deliberately — see existing project notes), the collision check
// still lives HERE, inside the movement-stepping logic, not in
// GameController.
// ---------------------------------------------------------------------------
namespace MovementSystem {

// ---- Grid movement -------------------------------------------------------

// Ported from GridMovementMechanics::update(). dt is unused — a grid
// hop is logically instantaneous; the visual lerp over time is
// RenderSystem's job (see GridRenderComponent), not this system's.
// Edge-trigger gate and all-or-nothing facing+position commit on a
// refused move are preserved exactly as before. position.x/y are
// tile-aligned floats (e.g. 3.0f) — cast to int at the point they're
// used as a tile index (see MovementSystem.cpp's nextGridPos).
void updateGrid(PositionComponent& position, GridMovementComponent& movement,
                 Direction inputDir,
                 const std::function<bool(const AABB&)>& isBlocked);

// World-pixel-space TILE_SIZE x TILE_SIZE rect at position's CURRENT
// grid cell. Same role as GridMovementMechanics::getHitbox().
AABB getGridHitbox(const PositionComponent& position);

// ---- Free movement --------------------------------------------------------

// Ported verbatim from FreeMovementMechanics::update(). Per-axis,
// collision-tested displacement; facing updates on any nonzero input
// regardless of whether that axis's move is actually accepted (matches
// original: `if (inputDir != Direction::NONE) position_.dir =
// inputDir;` ran before either axis's collision test).
void updateFree(PositionComponent& position, FreeMovementComponent& movement,
                 float dt, Direction inputDir,
                 const std::function<bool(const AABB&)>& isBlocked);

// World-pixel-space hitbox at position's CURRENT x/y. Same role as
// FreeMovementMechanics::getHitbox() / hitboxAt(x, y).
AABB getFreeHitbox(const PositionComponent& position, const FreeMovementComponent& movement);

// Hitbox the component WOULD have at an arbitrary (x, y) — exposed
// because updateFree's per-axis collision test needs to probe a
// candidate position before committing to it. Same role as
// FreeMovementMechanics' private hitboxAt() helper.
AABB freeHitboxAt(const FreeMovementComponent& movement, float x, float y);

} // namespace MovementSystem