#pragma once

#include "input/InputManager.h"
#include "asset/AssetDatabase.h"
#include "state/StateMachine.h"
#include "state/IGameState.h"
#include "events/EventQueue.h"
#include "system/AnimationSystem.h"
#include "time/GameClock.h"

struct GameServices
{
    InputManager& input;
    AssetDatabase& assets;
    StateMachine<IGameState>& states;
    AnimationSystem& animations;
    EventQueue& events;
    const GameClock& clock;
};