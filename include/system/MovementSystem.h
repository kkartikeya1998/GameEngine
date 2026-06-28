#pragma once

#include <functional>

#include "entities/Entity.h"
#include "entities/movement/Position2D.h"
#include "entities/movement/GridMovementComponent.h"
#include "entities/movement/FreeMovementComponent.h"

// ---------------------------------------------------------------------------
// MovementSystem — free functions holding the logic that used to live
// inside GridMovementMechanics::update() and FreeMovementMechanics::
// update(). Those classes are GONE; this is where their bodies moved
// to, ported field-for-field onto GridMovementComponent/
// FreeMovementComponent.
//
// Per the "collision check belongs in the mechanic" principle (kept
// deliberately — see existing project notes), the collision check
// still lives HERE, inside the movement-stepping logic, not in
// GameController. Only the OWNERSHIP shape changed (free function over
// a component, not a virtual method on a polymorphic mechanic) — the
// actual SOLID boundary (GameController stays collision-agnostic, just
// supplies isBlocked) is unchanged.
//
// Callers (GameController, and later an NPC behavior driver) dispatch
// by which movement component an Entity actually has:
//
//   if (auto* g = entity.get<GridMovementComponent>())
//       MovementSystem::updateGrid(*g, inputDir, isBlocked);
//   else if (auto* f = entity.get<FreeMovementComponent>())
//       MovementSystem::updateFree(*f, dt, inputDir, isBlocked);
//
// getHitbox()/getX()/getY()/isMoving() etc. that used to be uniform
// IMovementBehavior methods are now small free functions per component
// type (no shared interface needed — callers already know which
// component type they're holding, since they just branched on it to
// call the right update function).
// ---------------------------------------------------------------------------
namespace MovementSystem {

// ---- Grid movement -------------------------------------------------------

// Ported verbatim from GridMovementMechanics::update(). dt is unused —
// a grid hop is logically instantaneous; the visual lerp over time is
// RenderSystem's job (see GridRenderComponent), not this system's.
// Edge-trigger gate and all-or-nothing facing+position commit on a
// refused move are preserved exactly as before.
void updateGrid(GridMovementComponent& movement, Direction inputDir,
                 const std::function<bool(const AABB&)>& isBlocked);

// World-pixel-space TILE_SIZE x TILE_SIZE rect at the component's
// CURRENT grid cell. Same role as GridMovementMechanics::getHitbox().
AABB getGridHitbox(const GridMovementComponent& movement);

// ---- Free movement --------------------------------------------------------

// Ported verbatim from FreeMovementMechanics::update(). Per-axis,
// collision-tested displacement; facing updates on any nonzero input
// regardless of whether that axis's move is actually accepted (matches
// original: `if (inputDir != Direction::NONE) position_.dir =
// inputDir;` ran before either axis's collision test).
void updateFree(FreeMovementComponent& movement, float dt, Direction inputDir,
                 const std::function<bool(const AABB&)>& isBlocked);

// World-pixel-space hitbox at the component's CURRENT position. Same
// role as FreeMovementMechanics::getHitbox() / hitboxAt(x, y).
AABB getFreeHitbox(const FreeMovementComponent& movement);

// Hitbox the component WOULD have at an arbitrary (x, y) — exposed
// because updateFree's per-axis collision test needs to probe a
// candidate position before committing to it. Same role as
// FreeMovementMechanics' private hitboxAt() helper.
AABB freeHitboxAt(const FreeMovementComponent& movement, float x, float y);

} // namespace MovementSystem