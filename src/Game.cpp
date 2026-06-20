#include <iostream>

#include "Game.h"
#include "render/SFMLRenderer.h"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

sf::Clock gameClock;
sf::Time elapsed = gameClock.restart();

Game::Game()
    : controller_(1, 0, 0),
      atlas_(std::make_unique<SpriteAtlas>("C:\\Users\\kkart\\OneDrive\\Documents\\PokemonProject\\assets\\sprites\\npc.png")),
      renderSystem_(std::make_unique<RenderSystem>(std::make_unique<SFMLRenderer>(800, 600, *atlas_)))
{
}

void Game::run()
{
    try {
        while (renderSystem_->isOpen())
        {
            float dt = gameClock.restart().asSeconds();

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