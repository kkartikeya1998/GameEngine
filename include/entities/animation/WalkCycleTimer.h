#pragma once

// ---------------------------------------------------------------------------
// WalkCycleTimer — minimal time-based walk-cycle progress for continuous
// (free) movement.
//
// AnimationComponent is NOT reused here. Its entire model is "interpolate
// from a known start position to a known target position" (begin(), then
// progress 0->1 over time) — that fits grid movement (a move always has
// a discrete start and end tile) but has no equivalent in continuous
// movement, where position IS the live value already, with no "target"
// to lerp toward. Forcing FreeEntity through AnimationComponent would
// mean faking a target every frame for no benefit.
//
// What free movement actually needs from "animation" is much smaller:
// a repeating 0->1 progress value while moving, used the same way
// SpriteAtlas::frameFromProgress already consumes AnimationComponent's
// progress today — just driven by elapsed time instead of distance
// covered toward a destination.
// ---------------------------------------------------------------------------
class WalkCycleTimer {
public:
    explicit WalkCycleTimer(float cyclesPerSecond = 2.0f)
        : cyclesPerSecond_(cyclesPerSecond) {}

    // Call every frame with whether the entity is currently moving.
    void update(float dt, bool isMoving) {
        if (isMoving) {
            elapsed_ += dt;
        } else {
            elapsed_ = 0.f; // reset to standing frame when stopped
        }
    }

    // 0..1 repeating progress, suitable for SpriteAtlas::frameFromProgress.
    float getProgress() const {
        float cycle = elapsed_ * cyclesPerSecond_;
        return cycle - static_cast<float>(static_cast<int>(cycle));
    }

private:
    float cyclesPerSecond_;
    float elapsed_ = 0.f;
};