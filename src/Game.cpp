#include <iostream>

#include "Game.h"

Game::Game()
    : controller_(2, 1, 1),
      renderer_(std::make_unique<SFMLRenderer>(800, 600))
{
}

void Game::run()
{
    try {
        while (renderer_->isOpen())
        {
            // 1. input/events
            if (!renderer_->handleEvents())
                break;

            // 2. clear screen
            renderer_->clear();

            // 3. get world + map
            Map* map = controller_.getWorld()->getActiveMap();
            if (!map) continue;

            // 4. draw map
            for (int y = 0; y < map->getHeight(); y++) {
                for (int x = 0; x < map->getWidth(); x++) {
                    renderer_->drawTile(
                        x,
                        y,
                        map->tile_at(x, y).terrain()
                    );
                }
            }

            // 5. draw player
            Player* player = controller_.getPlayer();
            renderer_->drawPlayer(
                player->getX(),
                player->getY(),
                player->getDirection()
            );

            // 6. present frame
            renderer_->present();
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}