#include <iostream>

#include "Game.h"
#include "render/SFMLRenderer.h"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "system/GameConstants.h"

// NOTE: assetsRoot_ should point at the "assets" folder itself (the same
// folder that directly contains maps/ and sprites/). AssetManager builds
// its paths as assetsRoot_ + "/maps/...", "/sprites/...", and
// GameController forwards the same assetsRoot_ to World as
// assetsRoot_ + "/maps/". If PROJECT_ROOT previously pointed one level
// above assets/ (the project root, not the assets folder), append
// "/assets" here once instead of in every downstream consumer.
Game::Game()
    : assetsRoot_(std::string(PROJECT_ROOT) + "/assets")
    , assets_(assetsRoot_)
    , controller_(1, 600, 600, assetsRoot_, assets_.get<MapObjectRepository>()) // map ID: 1 main map, 2 asset tuning map
    , renderSystem_(std::make_unique<RenderSystem>(
          std::make_unique<SFMLRenderer>(
              GameConstants::GAME_RESOLUTION_W, GameConstants::GAME_RESOLUTION_H,
              assets_.get<TileRepository>(),
              assets_.get<MapObjectRepository>(),
              assets_.get<SpriteRepository>(),
              assetsRoot_ + "/objects/SimpleSummer_Tiles.png",
              assetsRoot_ + "/sprites/player_spritesheet.png",
              assetsRoot_ + "/objects/SimpleSummer_Objects.png"
          )
      ))
{
}

void Game::run()
{
    try {
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
                const sf::Event& event = *eventOpt;

                if (event.is<sf::Event::Closed>())
                    return;

                if (const auto* key = event.getIf<sf::Event::KeyPressed>())
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
            controller_.updatePlayerMovement(dt, dir);

            controller_.update(dt);

            // RENDER — now takes dt: RenderSystem steps each entity's
            // render-state component (lerp progress / walk-cycle
            // elapsed time) inline as part of drawing, a deliberate
            // tradeoff made when folding render-state stepping into
            // render() instead of a separate per-frame system pass.
            // See RenderSystem.cpp's ECS-ported version.
            renderSystem_->render(controller_, dt);
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}