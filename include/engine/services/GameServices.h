#pragma once

#include "engine/input/InputManager.h"
#include "engine/assets/AssetDatabase.h"
#include "game/state/StateMachine.h"
#include "game/state/IGameState.h"
#include "engine/events/EventQueue.h"
#include "game/ecs/animation/AnimationSystem.h"
#include "engine/time/GameClock.h"

struct GameServices
{
    InputManager& input;
    AssetDatabase& assets;
    StateMachine<IGameState>& states;
    AnimationSystem& animations;
    EventQueue& events;
    const GameClock& clock;
};