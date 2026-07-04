#include <iostream>

#include "Game.h"
#include "render/SFMLRenderer.h"
#include "state/GameplayState.h"
#include "system/GameConstants.h"

Game::Game()
    : assets_(),
      renderSystem_(std::make_unique<RenderSystem>(
          std::make_unique<SFMLRenderer>(
              GameConstants::GAME_RESOLUTION_W, GameConstants::GAME_RESOLUTION_H,
              assets_.get<TileRepository>(),
              assets_.get<MapObjectRepository>(),
              assets_.get<CharacterRepository>(),
              Assets::Objects::SIMPLE_SUMMER_TILES,
              Assets::Sprites::PLAYER_SPRITESHEET,
              Assets::Objects::SIMPLE_SUMMER_OBJECTS)))
{
    // Goes straight to gameplay for now — swap for pushing a
    // MainMenuState once one exists; this is the only line that changes.
    states_.Push(std::make_unique<GameplayState>(input_, assets_, assetsRoot_));
}

void Game::Update(float dt)
{
    states_.Update(dt);
}

void Game::Render()
{
    states_.Render(*renderSystem_, lastDt_);
}

void Game::Run()
{
    try
    {
        while (renderSystem_->isOpen())
        {
            lastDt_ = gameClock_.restart().asSeconds();

            // Window-close stays here rather than in InputManager —
            // the SFML event queue is owned by SFMLRenderer's window,
            // which I don't have direct access to without
            // SFMLRenderer.h. InputManager stays a pure key/mouse
            // poller per the design.
            while (auto eventOpt = renderSystem_->pollEvent())
            {
                if (eventOpt->is<sf::Event::Closed>())
                    return;
            }

            input_.PollEvents();
            Update(lastDt_);
            Render();
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
}