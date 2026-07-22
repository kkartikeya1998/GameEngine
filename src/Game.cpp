#include "Game.h"
#include "render/SFMLRenderer.h"
#include "state/GameplayState.h"
#include "system/GameConstants.h"
#include "system/InventorySystem.h"
#include "asset/AsssetPaths.h"

Game::Game()
    : assets_(),
      renderSystem_(std::make_unique<RenderSystem>(
          std::make_unique<SFMLRenderer>(
              GameConstants::GAME_RESOLUTION_W, GameConstants::GAME_RESOLUTION_H),
          assets_.renderRepository(),
          Assets::Objects::SIMPLE_SUMMER_TILES)),
      animationSystem_(assets_),
      interactions_(assets_, states_, input_, events_),
      dispatcher_(events_, assets_, *this, interactions_)
{
    // straight to gameplay mode
    states_.Push(std::make_unique<GameplayState>(
        GameServices{
            input_,
            assets_,
            states_,
            animationSystem_,
            events_},
        Assets::Fonts::PIXFAY));
}

Registry *Game::GetRegistry() const
{
    return states_.FindFirst(
        [](IGameState *state)
        {
            return state->GetRegistry();
        });
}

void Game::Update(float dt)
{
    states_.Update(dt);

    dispatcher_.Process();

    interactions_.Update(dt);
}

void Game::Render()
{
    states_.Render(*renderSystem_, lastDt_);
}

void Game::Run()
{
    // No try/catch here by design: everything this loop can throw is an
    // EngineException-derived, unrecoverable failure (see
    // exceptions/EngineExceptions.h) — asset/renderer init already happened
    // successfully by the time we're here, so what's left to propagate is
    // things like a corrupted map file mid-transition. Those belong at the
    // single top-level boundary in main.cpp, which logs once and exits
    // with a non-zero code. Catching and logging here too would let Run()
    // return normally after a fatal error, so main() would report success
    // (exit code 0) on a crash — see main.cpp.
    LOG_INFO("Engine startup complete. Entering main loop.");
    while (renderSystem_->isOpen())
    {
        lastDt_ = gameClock_.restart().asSeconds();

        // first check for qindow close events before polling input, so we don't miss a quit request
        while (auto event = renderSystem_->pollEvent())
        {
            input_.ProcessEvent(*event);
        }

        if (input_.ShouldQuit())
        {
            break; // fall through to the shutdown log below, instead of returning past it
        }

        input_.PollEvents();

        Update(lastDt_);
        Render();
    }
    LOG_INFO("Engine shutting down.");
}