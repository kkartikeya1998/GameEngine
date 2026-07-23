#include "Game.h"
#include "render/SFMLRenderer.h"
#include "state/GameplayState.h"
#include "system/GameConstants.h"
#include "system/InventorySystem.h"
#include "asset/AsssetPaths.h"
#include "state/StateFactory.h"

Game::Game()
    : assets_(),
      renderSystem_(std::make_unique<RenderSystem>(
          std::make_unique<SFMLRenderer>(
              GameConstants::GAME_RESOLUTION_W, GameConstants::GAME_RESOLUTION_H),
          assets_.renderRepository(),
          Assets::Objects::SIMPLE_SUMMER_TILES)),
      animationSystem_(assets_),
      interactions_(GameServices{input_, assets_, states_, animationSystem_, events_}),
      dispatcher_(events_, assets_, *this, interactions_)
{
    // straight to gameplay mode
    states_.Push(StateFactory::MakeGameplay(
        GameServices{
            input_, assets_, states_, animationSystem_, events_}));
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
            break;
        }

        input_.PollEvents();

        Update(lastDt_);
        Render();
    }
    LOG_INFO("Engine shutting down.");
}