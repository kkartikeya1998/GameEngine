#pragma once

#include "tmp/animation/AnimationComponent.h"
#include "render/IRenderState.h"

// ---------------------------------------------------------------------------
// AnimationComponentAdapter — wraps the EXISTING, UNCHANGED
// AnimationComponent to satisfy IRenderState.
//
// AnimationComponent itself is not modified at all — it has no concept
// of "did position change," it only knows begin(start, target) and
// update(dt)/getProgress(). This adapter is what detects a logical
// position change frame-to-frame and translates that into a begin()
// call — that detection used to live inside GridEntity::move() (which
// called animation_.begin() directly at the moment a hop committed).
// Now that GridMovementMechanics commits hops inside its own update()
// rather than a separate move() entry point Entity can hook, detecting
// "did the hop happen" has to happen here instead, by comparing this
// frame's logicalX/Y against last frame's.
//
// This is the ONLY new logic introduced for grid rendering — everything
// else (the lerp math, getProgress, isAnimating) is the original
// AnimationComponent, untouched.
// ---------------------------------------------------------------------------
class AnimationComponentAdapter : public IRenderState {
public:
    explicit AnimationComponentAdapter(float speed = 6.0f)
        : animation_(speed)
    {
    }

    void update(float dt, float logicalX, float logicalY, bool /*isMoving*/) override
    {
        if (!hasSeenPosition_) {
            // First frame: nothing to lerp from. Snap baseline so we
            // don't spuriously animate from (0,0) on entity spawn.
            lastLogicalX_ = logicalX;
            lastLogicalY_ = logicalY;
            hasSeenPosition_ = true;
        } else if (logicalX != lastLogicalX_ || logicalY != lastLogicalY_) {
            // Position changed since last frame -> a hop just committed.
            // Begin a lerp from where we last rendered toward the new
            // logical position, exactly as GridEntity::move() used to
            // trigger animation_.begin(start, target) at hop time.
            animation_.begin(lastLogicalX_, lastLogicalY_, logicalX, logicalY);
            lastLogicalX_ = logicalX;
            lastLogicalY_ = logicalY;
        }

        animation_.update(dt);
    }

    float getRenderX() const override
    {
        return animation_.isAnimating() ? animation_.getRenderX() : lastLogicalX_;
    }

    float getRenderY() const override
    {
        return animation_.isAnimating() ? animation_.getRenderY() : lastLogicalY_;
    }

    float getProgress() const override { return animation_.getProgress(); }

private:
    AnimationComponent animation_;
    float lastLogicalX_ = 0.f;
    float lastLogicalY_ = 0.f;
    bool hasSeenPosition_ = false;
};