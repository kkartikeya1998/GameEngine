#pragma once

#include "entities/movement/Position2D.h"

// ---------------------------------------------------------------------------
// IRenderState — strategy interface for "how does this entity's logical
// position/movement turn into what gets drawn."
//
// THIS IS THE NEW AXIS this redesign introduces. Previously this was
// implied entirely by which Entity subclass you had: GridEntity always
// meant AnimationComponent's lerp-to-target model, FreeEntity always
// meant WalkCycleTimer's elapsed-phase model. That coupling is exactly
// what made movement-family and render-family impossible to mix (e.g.
// you could not give a grid-moving NPC SCML skeletal animation without
// writing a new Entity subclass). IRenderState makes render/animation
// strategy an injected component, symmetric with how IMovementBehavior
// already made movement strategy injected.
//
// update() is called every frame with the entity's CURRENT logical
// position (post-IMovementBehavior::update, i.e. authoritative,
// already-resolved position for this frame) and whether the entity
// moved this frame. Each concrete IRenderState decides what to do with
// that:
//
//   - AnimationComponentAdapter (wraps the existing AnimationComponent):
//     on the frame logicalX/Y CHANGES versus what it saw last frame, it
//     calls animation_.begin(lastSeenX, lastSeenY, logicalX, logicalY)
//     and lerps toward it over time — this is the grid-hop visual slide,
//     unchanged from today's GridEntity behavior. On frames where
//     position doesn't change, it just ticks the in-flight lerp (or sits
//     idle if not animating).
//
//   - WalkCycleTimerAdapter (wraps the existing WalkCycleTimer):
//     ignores logicalX/Y entirely (free movement's position is already
//     continuous/live, nothing to lerp toward) and just advances elapsed
//     time while isMoving is true, exactly as WalkCycleTimer always did.
//
//   - A future SCML/AnimationController-backed implementation can pick
//     either model (or its own) — Entity, GameController, and
//     RenderSystem never change either way.
//
// getRenderX()/getRenderY() take the SAME logical position as a
// fallback/baseline — a render state with nothing in flight just
// returns it unchanged (this is what makes AnimationComponentAdapter
// degrade correctly to "no visual lag" when not mid-hop, and is exactly
// how GridEntity::getRenderX already behaved: animation_.isAnimating()
// ? lerped : raw position).
// ---------------------------------------------------------------------------
class IRenderState {
public:
    virtual ~IRenderState() = default;

    // Called every frame, after movement has resolved. logicalX/Y is the
    // entity's authoritative current position this frame (whatever
    // IMovementBehavior::getX/getY returned). isMoving mirrors
    // IMovementBehavior::isMoving() for this frame.
    virtual void update(float dt, float logicalX, float logicalY, bool isMoving) = 0;

    // Position to actually draw at. Equals (logicalX, logicalY) passed
    // into the most recent update() unless this render state is
    // currently interpolating away from it (grid-hop slide).
    virtual float getRenderX() const = 0;
    virtual float getRenderY() const = 0;

    // 0..1 progress value for frame-selection (SpriteAtlas::frameFromProgress
    // or equivalent). Meaning is strategy-specific (lerp-progress for
    // AnimationComponentAdapter, repeating walk-phase for
    // WalkCycleTimerAdapter) — callers needing frame selection just want
    // *a* 0..1 value consistently named the same way regardless of which
    // strategy is active, same way Entity::getAnimationProgress() and
    // FreeEntity::getWalkProgress() served the same purpose under two
    // different names before.
    virtual float getProgress() const = 0;
};