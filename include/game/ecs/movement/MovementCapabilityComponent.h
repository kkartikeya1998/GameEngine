#pragma once
#include "engine/ecs/Component.h"
#include "game/ecs/movement/MovementMode.h"

// Absence of this component means Walking-only — safe default for every
// entity that doesn't explicitly need Surf/Fly.
struct MovementCapabilityComponent : public Component {
    MovementMode flags = MovementMode::Walking;

    MovementCapabilityComponent() = default;
    explicit MovementCapabilityComponent(MovementMode flags) : flags(flags) {}
};