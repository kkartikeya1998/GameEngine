#pragma once

#include <memory>
#include <string>
#include <SFML/System/Clock.hpp>

#include "asset/AssetManager.h"
#include "input/InputManager.h"
#include "state/StateMachine.h"
#include "render/RenderSystem.h"

// ---------------------------------------------------------------------------
// Game — top-level owner of long-lived systems and the main loop.
//
// ---------------------------------------------------------------------------
class Game {
public:
    Game();

    void Run();

private:
    std::string assetsRoot_;

    AssetManager assets_;
    InputManager input_;
    StateMachine states_;
    std::unique_ptr<RenderSystem> renderSystem_;

    sf::Clock gameClock_;
    float lastDt_ = 0.f;

    void Update(float dt);
    void Render();
};