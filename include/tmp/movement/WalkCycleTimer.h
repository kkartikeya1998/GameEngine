#pragma once

#include "entities/Component.h"

// ---------------------------------------------------------------------------
// WalkCycleTimer — minimal time-based walk-cycle progress for continuous
// (free) movement. Attached as a component; owns its own tiny update logic
// the same way CollisionComponent owns resolve().
// ---------------------------------------------------------------------------
struct WalkCycleTimer : public Component {
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