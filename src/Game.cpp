#include <iostream>

#include "Game.h"

Game::Game()
    : controller_(1, 0, 0),
      renderer_(std::make_unique<SFMLRenderer>(800, 600))
{
}

void Game::run()
{
    try {
        while (renderer_->isOpen())
        {
            // -------------------------
            // 1. INPUT
            // -------------------------
            Direction dir = Direction::NONE;
            bool moveRequested = false;

            while (auto eventOpt = renderer_->pollEvent())
            {
                const sf::Event& event = *eventOpt;

                if (event.is<sf::Event::Closed>())
                {
                    return;
                }

                if (const auto* key = event.getIf<sf::Event::KeyPressed>())
                {
                    if (key->code == sf::Keyboard::Key::Escape)
                    {
                        return;
                    }
                    
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

            // -------------------------
            // 2. UPDATE
            // -------------------------
            if (moveRequested && dir != Direction::NONE)
            {
                controller_.movePlayer(dir);
            }

            // -------------------------
            // 3. RENDER
            // -------------------------
            renderer_->clear();

            Map* map = controller_.getWorld()->getActiveMap();
            if (!map) {
                renderer_->present();
                continue;
            }

            for (int y = 0; y < map->getHeight(); y++) {
                for (int x = 0; x < map->getWidth(); x++) {
                    renderer_->drawTile(
                        x,
                        y,
                        map->tile_at(x, y).terrain()
                    );
                }
            }

            Player* player = controller_.getPlayer();
            renderer_->drawPlayer(
                player->getX(),
                player->getY(),
                player->getDirection()
            );

            renderer_->present();
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}