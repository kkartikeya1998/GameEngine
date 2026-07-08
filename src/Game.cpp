#include <iostream>

#include "Game.h"
#include "render/SFMLRenderer.h"
#include "state/GameplayState.h"
#include "system/GameConstants.h"

Game::Game()
    : assets_(),
      renderSystem_(std::make_unique<RenderSystem>(
          std::make_unique<SFMLRenderer>(
              GameConstants::GAME_RESOLUTION_W, GameConstants::GAME_RESOLUTION_H),
          assets_.get<TileRepository>(),
          Assets::Objects::SIMPLE_SUMMER_TILES)),
      animationSystem_(assets_.get<CharacterRepository>())
{
    // straight to gameplay mode
    states_.Push(std::make_unique<GameplayState>(input_, assets_, states_, animationSystem_));
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
        while (renderSystem_->isOpen()){
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
        std::cout << "Error: " << e.what() << "\n";
    }
}