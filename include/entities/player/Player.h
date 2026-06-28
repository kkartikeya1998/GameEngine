#pragma once

#include <memory>

#include "entities/Entity.h"
#include "entities/movement/FreeMovementMechanics.h"
#include "render/WalkCycleTimerAdapter.h"

// Player is just an alias for the one controlled Entity in ECS.
using Player = Entity;

inline Player makePlayer(
    std::unique_ptr<FreeMovementMechanics> movement,
    float walkCyclePerSeconds = 4.0f)
{
    return Player(
        std::move(movement),
        std::make_unique<WalkCycleTimerAdapter>(walkCyclePerSeconds)
    );
}