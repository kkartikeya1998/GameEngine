#pragma once
#include "entities/Component.h"

// ---------------------------------------------------------------------------
// FreeMovementComponent — pure data for "how this entity moves":
// speed and its sprint scale
// ---------------------------------------------------------------------------
struct FreeMovementComponent : public Component {
    float speed = 0.f;
    float sprintMultiplier = 1.5f;

    FreeMovementComponent() = default;
    FreeMovementComponent(float speed, float sprintMultiplier = 1.5f)
        : speed(speed), sprintMultiplier(sprintMultiplier)
    {}
};