#pragma once
#include "entities/Component.h"

enum class MovementState {
    Idle,
    Walking,
    Sprinting
};

struct MovementStateComponent : public Component {
    MovementState current = MovementState::Idle;
};