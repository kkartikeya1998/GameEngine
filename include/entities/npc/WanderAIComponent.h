// WanderAIComponent.h
#pragma once
#include "entities/Component.h"

enum class CreatureState {
    Idle,
    Moving,
    Jumping
};

// AI-driven counterpart to PlayerControlComponent: this is what
// decides velocity/direction each frame for a non-player creature.
// Pure state — all logic lives in CreatureAISystem.
struct WanderAIComponent : public Component {
    CreatureState state = CreatureState::Idle;
    float stateTimer = 0.f;
    float targetX = 0.f;
    float targetY = 0.f;
    float movingElapsed = 0.f;
    float wanderSpeed = GameConstants::WILD_POKEMON_SPEED; // per-entity — lets EntityFactory vary by species

    WanderAIComponent() = default;
};