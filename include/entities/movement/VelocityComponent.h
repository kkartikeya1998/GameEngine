#pragma once
#include "entities/Component.h"

struct VelocityComponent : public Component {
    float vx = 0.f;
    float vy = 0.f;

    VelocityComponent() = default;
    VelocityComponent(float vx, float vy) : vx(vx), vy(vy) {}
};