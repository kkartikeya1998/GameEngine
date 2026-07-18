#include <iostream>

#include "Game.h"
#include "render/SFMLRenderer.h"
#include "state/GameplayState.h"
#include "system/GameConstants.h"
#include "system/InventorySystem.h"
#include "log/Logger.h"

Game::Game()
    : assets_(),
      renderSystem_(std::make_unique<RenderSystem>(
          std::make_unique<SFMLRenderer>(
              GameConstants::GAME_RESOLUTION_W, GameConstants::GAME_RESOLUTION_H),
          assets_.renderRepository(),
          Assets::Objects::SIMPLE_SUMMER_TILES)),
      animationSystem_(assets_),
      interactions_(assets_, states_, input_, events_)
{
    // straight to gameplay mode
    states_.Push(std::make_unique<GameplayState>(input_, assets_, states_, animationSystem_, events_));
}

void Game::Update(float dt)
{
    states_.Update(dt);
    events_.Drain([this](auto &&e) { // [TODO] NEEDS FIXING/ delegation
        using T = std::decay_t<decltype(e)>;
        LOG_TRACE(std::string("Draining event: ") + typeid(T).name());
        if constexpr (std::is_same_v<T, ItemConsumed>)
        {
            if (Registry *reg = states_.FindFirst([](IGameState *s)
                                                  { return s->GetRegistry(); }))
                InventorySystem::handleItemConsumed(*reg, e);
        }
        else if constexpr (std::is_same_v<T, InteractionRequested>)
        {
            interactions_.HandleRequested(e);
        }
        else if constexpr (std::is_same_v<T, DialogueFinished>)
        {
            interactions_.HandleDialogueFinished(e);
        }
        else if constexpr (std::is_same_v<T, BattleFinished>)
        {
            interactions_.HandleBattleFinished(e);
        }
        else if constexpr (std::is_same_v<T, ItemPickedUp>)
        {
            if (Registry *reg = states_.FindFirst([](IGameState *s)
                                                  { return s->GetRegistry(); }))
            {
                if (auto *inv = reg->get<InventoryComponent>(e.owner))
                    InventorySystem::addItem(*inv, assets_, e.itemId, e.quantity);
            }
        }
    });
    interactions_.Update(dt); // new
}

void Game::Render()
{
    states_.Render(*renderSystem_, lastDt_);
}

void Game::Run()
{
    LOG_INFO("Engine startup complete. Entering main loop.");
    try
    {
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
                return;
            }

            input_.PollEvents();

            Update(lastDt_);
            Render();
        }
    }
    catch (const std::exception &e)
    {
        LOG_FATAL(std::string("Unhandled exception in main loop: ") + e.what());
    }
    LOG_INFO("Engine shutting down.");
}
