#pragma once

#include "tmp/animation/WalkCycleTimer.h"
#include "render/IRenderState.h"

// ---------------------------------------------------------------------------
// WalkCycleTimerAdapter — wraps the EXISTING, UNCHANGED WalkCycleTimer
// to satisfy IRenderState.
//
// logicalX/Y are ignored entirely, same as before — free movement's
// position is already continuous and live (FreeEntity::getRenderX
// already just returned movement_->getPosition().x with "no
// interpolation needed" per its own old comment). getRenderX/Y here
// simply echo back whatever logical position update() was last given.
// ---------------------------------------------------------------------------
class WalkCycleTimerAdapter : public IRenderState {
public:
    explicit WalkCycleTimerAdapter(float cyclesPerSecond = 2.0f)
        : walkCycle_(cyclesPerSecond)
    {
    }

    void update(float dt, float logicalX, float logicalY, bool isMoving) override
    {
        lastLogicalX_ = logicalX;
        lastLogicalY_ = logicalY;
        walkCycle_.update(dt, isMoving);
    }

    float getRenderX() const override { return lastLogicalX_; }
    float getRenderY() const override { return lastLogicalY_; }
    float getProgress() const override { return walkCycle_.getProgress(); }

private:
    WalkCycleTimer walkCycle_;
    float lastLogicalX_ = 0.f;
    float lastLogicalY_ = 0.f;
};