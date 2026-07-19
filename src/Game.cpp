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
            else
                LOG_WARNING("Game: ItemConsumed drained but no active state has a Registry — event dropped");
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
                else
                    LOG_WARNING(std::format("Game: ItemPickedUp for entity {} with no InventoryComponent — item dropped", e.owner.index));
            }
            else
            {
                LOG_WARNING("Game: ItemPickedUp drained but no active state has a Registry — event dropped");
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