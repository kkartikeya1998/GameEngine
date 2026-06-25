#pragma once

#include <functional>
#include <memory>

#include "entities/Entity.h"
#include "entities/animation/WalkCycleTimer.h"
#include "entities/movement/IFreeMovementMechanics.h"

// ---------------------------------------------------------------------------
// FreeEntity — continuous, non-grid-snapped movement.
//
// updateMovement() REPLACES GridEntity::move(dir) as the per-frame entry
// point. GameController should call this every frame (not once per
// keypress) with the current held-direction input and a collision query
// built from the active Map. This is a structural difference from
// GridEntity, not just a renamed method — see IFreeMovementMechanics.h.
//
// update(float dt) overrides Entity's default (which drives
// AnimationComponent) — FreeEntity never touches animation_ at all.
// AnimationComponent's lerp-to-target model has nothing to interpolate
// here (position is already continuous, live, no target). Instead,
// walkCycle_ tracks elapsed-time-while-moving for walk-frame selection —
// see WalkCycleTimer.h. getWalkProgress() exposes that for the renderer;
// the base class's isAnimating()/getAnimationProgress() are NOT
// meaningful for FreeEntity (they'd report AnimationComponent's
// always-idle state) — callers that need to distinguish should use
// getWalkProgress()/isMoving() instead for this entity type.
// ---------------------------------------------------------------------------
class FreeEntity : public Entity {
public:
    explicit FreeEntity(std::unique_ptr<IFreeMovementMechanics> movement,
                         float walkCyclesPerSecond = 2.0f);

    // Called every frame. inputDir is this frame's held-direction input
    // (NONE if nothing held). isBlocked is built by the caller from the
    // active Map (see Map::isAreaBlocked) + render TILE_SIZE.
    void updateMovement(float dt, Direction inputDir,
                         const std::function<bool(const AABB&)>& isBlocked);

    // Overrides Entity's default — does NOT drive AnimationComponent.
    // Advances the walk-cycle timer based on the movement mechanic's
    // current isMoving() state.
    void update(float dt) override;

    float getX() const;
    float getY() const;

    Direction getDirection() const override;
    float getRenderX() const override;
    float getRenderY() const override;

    AABB getHitbox() const;
    bool isMoving() const;

    // Walk-cycle progress (0..1, repeating) for SpriteAtlas::frameFromProgress.
    // Use this instead of the base class's getAnimationProgress() for
    // FreeEntity — see class comment above.
    float getWalkProgress() const;

    IFreeMovementMechanics& movement();
    const IFreeMovementMechanics& movement() const;

private:
    std::unique_ptr<IFreeMovementMechanics> movement_;
    WalkCycleTimer walkCycle_;
};