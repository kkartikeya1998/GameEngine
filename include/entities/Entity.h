#pragma once

#include "entities/movement/Position.h"
#include "animation/AnimationComponent.h"

// ---------------------------------------------------------------------------
// Entity — shared base for anything renderable with a facing direction.
//
// GridEntity and FreeEntity (siblings, both deriving from this) each
// drive their own movement mechanic correctly. Entity itself only keeps
// what's universally true: a facing direction and a renderable position,
// both as pure virtual queries every concrete entity must answer.
//
// AnimationComponent stays a member here (not moved into GridEntity)
// because update(float dt) has a default body that drives it — GridEntity
// uses that default as is; FreeEntity overrides update() entirely and
// never touches animation_ (it uses WalkCycleTimer instead — see
// FreeEntity.h for why AnimationComponent doesn't fit free movement).
// ---------------------------------------------------------------------------
class Entity {
public:
    virtual ~Entity() = default;

    // Default: drives AnimationComponent's interpolation timer. Correct
    // for GridEntity as-is. FreeEntity overrides this entirely.
    virtual void update(float dt) { animation_.update(dt); }

    virtual Direction getDirection() const = 0;

    virtual float getRenderX() const = 0;
    virtual float getRenderY() const = 0;

    bool isAnimating() const { return animation_.isAnimating(); }
    float getAnimationProgress() const { return animation_.getProgress(); }

protected:
    AnimationComponent animation_{6.0f};
};