#pragma once

#include <memory>
#include <string>
#include <SFML/System/Clock.hpp>

#include "engine/assets/AssetDatabase.h"
#include "engine/input/InputManager.h"
#include "game/state/StateMachine.h"
#include "engine/render/RenderSystem.h"
#include "game/state/IGameState.h"
#include "system/AnimationSystem.h"
#include "interaction/InteractionManager.h"
#include "engine/events/EventQueue.h"
#include "engine/events/EventDispatcher.h"
#include "game/IGameContext.h"
#include "engine/time/GameClock.h"
#include "engine/logging/Logger.h"
// ---------------------------------------------------------------------------
// Game — top-level owner of long-lived systems and the main loop.
//
// ---------------------------------------------------------------------------
class Game : public IGameContext{
public:
    Game();

    void Run();

    Registry* GetRegistry() const override;

private:
    std::string assetsRoot_;

    AssetDatabase assets_;
    InputManager input_;
    StateMachine<IGameState> states_;
    AnimationSystem animationSystem_;
    EventQueue events_;
    EventDispatcher dispatcher_;
    InteractionManager interactions_;
    std::unique_ptr<RenderSystem> renderSystem_;

    sf::Clock wallClock_;   // renamed from gameClock_ — raw OS clock, private to Game
    GameClock clock_;       // canonical engine clock

    void Update(const GameClock& clock);
    void Render();
};