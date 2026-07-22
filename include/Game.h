#pragma once

#include <memory>
#include <string>
#include <SFML/System/Clock.hpp>

#include "asset/AssetDatabase.h"
#include "input/InputManager.h"
#include "state/StateMachine.h"
#include "render/RenderSystem.h"
#include "state/IGameState.h"
#include "system/AnimationSystem.h"
#include "interaction/InteractionManager.h"
#include "events/EventQueue.h"
#include "events/EventDispatcher.h"
#include "game/IGameContext.h"
#include "log/Logger.h"
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

    sf::Clock gameClock_;
    float lastDt_ = 0.f;

    void Update(float dt);
    void Render();
};