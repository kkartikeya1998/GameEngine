#include <iostream>

#include "Game.h"
#include "render/RenderSystem.h"
#include "render/SFMLRenderer.h"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "system/GameConstants.h"

Game::Game()
    : assets_(),
      controller_(1, 600, 600, // map ID: 1 main map, 2 asset tuning map
                  assetsRoot_, assets_.get<MapObjectRepository>(),
                  assets_.get<TileRepository>()),
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
}

void Game::run()
{
    try
    {
        while (renderSystem_->isOpen())
        {
            float dt = gameClock_.restart().asSeconds();

            // EVENTS — kept for one-shot, edge-triggered things only:
            // window close, Escape-to-quit. Movement no longer reacts to
            // KeyPressed events (that only fires once per physical press,
            // which is wrong for continuous movement — holding a key
            // should keep moving the player every frame it's held).
            while (auto eventOpt = renderSystem_->pollEvent())
            {
                const sf::Event &event = *eventOpt;

                if (event.is<sf::Event::Closed>())
                    return;

                if (const auto *key = event.getIf<sf::Event::KeyPressed>())
                {
                    if (key->code == sf::Keyboard::Key::Escape)
                        return;
                }
            }

            // NOTE: this picks one direction per frame (last-checked-wins
            // if multiple keys are held, since these are plain ifs, not a
            // combined vector). If you want true 8-directional/diagonal
            // movement, this would need to combine multiple held keys
            // into one input vector instead of picking a single
            // Direction
            Direction dir = Direction::NONE;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                dir = Direction::UP;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                dir = Direction::DOWN;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                dir = Direction::LEFT;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                dir = Direction::RIGHT;

            // UPDATE — called every frame regardless of whether dir is
            // NONE, so the player can decelerate/stop cleanly (handled
            // inside FreeMovementMechanics::update). The old
            // isAnimating()-gate is gone: that existed to stop a new
            // discrete hop from interrupting an in-progress one, which
            // has no equivalent for continuous movement.
            std::cout << "[Game]: Updating player movement with input dir: " << static_cast<int>(dir) << "\n"; 
            controller_.updatePlayerMovement(dt, dir);
            std::cout << "[Game]: Player movement updated.\n";

            controller_.update(dt);

            // RENDER — now takes dt: RenderSystem steps each entity's
            // render-state component (lerp progress / walk-cycle
            // elapsed time) inline as part of drawing, a deliberate
            // tradeoff made when folding render-state stepping into
            // render() instead of a separate per-frame system pass.
            // See RenderSystem.cpp's ECS-ported version.
            std::cout << "[Game]: Rendering frame with dt: " << dt << "\n";
            renderSystem_->render(controller_, dt);
            std::cout << "[Game]: Frame rendered.\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
}