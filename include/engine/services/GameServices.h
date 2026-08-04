#pragma once

#include "engine/input/InputManager.h"
#include "engine/assets/AssetDatabase.h"
#include "game/state/StateMachine.h"
#include "game/state/IGameState.h"
#include "engine/events/EventQueue.h"
#include "engine/time/GameClock.h"

template <typename T>
struct GameServices
{
    InputManager& input;
    AssetDatabase& assets;
    StateMachine<IGameState>& states;
    EventQueue<T>& events;
    const GameClock& clock;
};