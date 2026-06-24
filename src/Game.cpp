#include <iostream>

#include "Game.h"
#include "render/SFMLRenderer.h"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

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
    , controller_(1, 0, 0, assetsRoot_, assets_.get<MapObjectRepository>())
    , renderSystem_(std::make_unique<RenderSystem>(
          std::make_unique<SFMLRenderer>(
              800, 600,
              assets_.get<TileRepository>(),
              assets_.get<MapObjectRepository>(),
              assets_.get<SpriteRepository>(),
              assetsRoot_ + "/maps/path_tileset.png",
              assetsRoot_ + "/sprites/player_spritesheet.png",
              assetsRoot_ + "/maps/openworld.png"
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

            // INPUT
            Direction dir = Direction::NONE;
            bool moveRequested = false;

            while (auto eventOpt = renderSystem_->pollEvent())
            {
                const sf::Event& event = *eventOpt;

                if (event.is<sf::Event::Closed>())
                    return;

                if (const auto* key = event.getIf<sf::Event::KeyPressed>())
                {
                    if (key->code == sf::Keyboard::Key::Escape)
                        return;

                    switch (key->code)
                    {
                        case sf::Keyboard::Key::W:
                        case sf::Keyboard::Key::Up:
                            dir = Direction::UP;
                            moveRequested = true;
                            break;

                        case sf::Keyboard::Key::S:
                        case sf::Keyboard::Key::Down:
                            dir = Direction::DOWN;
                            moveRequested = true;
                            break;

                        case sf::Keyboard::Key::A:
                        case sf::Keyboard::Key::Left:
                            dir = Direction::LEFT;
                            moveRequested = true;
                            break;

                        case sf::Keyboard::Key::D:
                        case sf::Keyboard::Key::Right:
                            dir = Direction::RIGHT;
                            moveRequested = true;
                            break;

                        default:
                            break;
                    }
                }
            }

            // UPDATE
            if (moveRequested && dir != Direction::NONE && !controller_.getPlayer()->isAnimating())
                controller_.movePlayer(dir);

            controller_.update(dt);

            // RENDER
            renderSystem_->render(controller_);
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}