#include "Game.h"
#include "engine/render/SFMLRenderer.h"
#include "game/state/GameplayState.h"
#include "game/GameConstants.h"
#include "game/ecs/inventory/InventorySystem.h"
#include "engine/assets/AssetPaths.h"
#include "game/state/StateFactory.h"

Game::Game()
    : assets_(),
      renderSystem_(std::make_unique<RenderSystem>(
          std::make_unique<SFMLRenderer>(
              GameConstants::GAME_RESOLUTION_W, GameConstants::GAME_RESOLUTION_H),
          assets_.renderRepository(),
          Assets::Objects::SIMPLE_SUMMER_TILES)),
      interactions_(GameServices{input_, assets_, states_, events_, clock_}),
      dispatcher_(events_, assets_, *this, interactions_)
{
    states_.Push(StateFactory::MakeGameplay(
        GameServices{
            input_, assets_, states_, events_, clock_}));
}

Registry *Game::GetRegistry() const
{
    return states_.FindFirst(
        [](IGameState *state)
        {
            return state->GetRegistry();
        });
}

void Game::Update(const GameClock &clock)
{
    states_.Update(static_cast<float>(clock.DeltaTime()));

    dispatcher_.Process();

    interactions_.Update();
}

void Game::Render()
{
    states_.Render(*renderSystem_, static_cast<float>(clock_.DeltaTime()));
}

void Game::Run()
{
    LOG_INFO("Engine startup complete. Entering main loop.");
    while (renderSystem_->isOpen())
    {
        clock_.BeginFrame(wallClock_.restart().asSeconds());
        Logger::SetFrameNumber(clock_.FrameNumber());

        while (auto event = renderSystem_->pollEvent())
        {
            input_.ProcessEvent(*event);
        }

        if (input_.ShouldQuit())
        {
            break;
        }

        input_.PollEvents();

        // Fixed-step slot — no-op today, ready for Physics/AI FixedUpdate once those exist:
        // while (clock_.ShouldRunFixedUpdate())
        // {
        //     clock_.ConsumeFixedStep();
        // }

        Update(clock_);
        Render();

        clock_.EndFrame();
    }
    LOG_INFO("Engine shutting down.");
}