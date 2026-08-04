#pragma once

#include <memory>
#include <string>
#include <SFML/System/Clock.hpp>

#include "engine/assets/AssetDatabase.h"
#include "engine/input/InputManager.h"
#include "game/state/StateMachine.h"
#include "engine/render/RenderSystem.h"
#include "game/state/IGameState.h"
#include "engine/events/EventQueue.h"
#include "game/events/GameEvents.h"
#include "engine/time/GameClock.h"
#include "engine/logging/Logger.h"
// ---------------------------------------------------------------------------
// Game — top-level owner of long-lived systems and the main loop.
//
// ---------------------------------------------------------------------------
class Game{
public:
    Game();

    void Run();

    Registry* GetRegistry() const;

private:
    AssetDatabase assets_;
    InputManager input_;
    StateMachine<IGameState> states_;
    EventQueue<GameEvent> events_;
    std::unique_ptr<RenderSystem> renderSystem_;
    sf::Clock wallClock_;
    GameClock clock_;

    void Update(const GameClock& clock);
    void Render();
};