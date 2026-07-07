#pragma once
#include "entities/Component.h"

struct WalkCycleTimer : public Component
{
    explicit WalkCycleTimer(float cyclesPerSecond = 2.0f)
        : cyclesPerSecond_(cyclesPerSecond) {}

    void update(float dt, bool isMoving, float speedScale = 1.f)
    {
        // if (isMoving) elapsed_ += dt * speedScale;
        // else elapsed_ = 0.f;
        elapsed_ += dt * speedScale;
    }

    float getProgress() const
    {
        float cycle = elapsed_ * cyclesPerSecond_;
        return cycle - static_cast<float>(static_cast<int>(cycle));
    }

private:
    float cyclesPerSecond_;
    float elapsed_ = 0.f;
};