#pragma once
#include "entities/Registry.h"
#include "component/MovementStateComponent.h"
#include "component/AnimationComponent.h"
#include "log/Logger.h"

// ---------------------------------------------------------------------------
// MovementAnimationSystem — the ONLY place "movement state" maps to
// "which clip plays". AnimationPlayerSystem/AnimationSystem stays
// generic; this is the thin policy layer in front of it.
// Must run before AnimationSystem::update() each frame so a state
// change this frame is reflected in the same frame's advance.
// ---------------------------------------------------------------------------
namespace MovementAnimationSystem {

inline std::string clipNameFor(MovementState state)
{
    switch (state)
    {
        case MovementState::Walking:   return "walk";
        case MovementState::Sprinting: return "run";
        case MovementState::Idle:
        default:                       return "idle";
    }
}

inline void update(Registry &registry)
{
    for (EntityID id : registry.view<MovementStateComponent, AnimationComponent>())
    {
        auto &movement = *registry.get<MovementStateComponent>(id);
        auto &player   = *registry.get<AnimationComponent>(id);
        player.play(clipNameFor(movement.current));
    }
}

}