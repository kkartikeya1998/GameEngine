#include "render/RenderSystem.h"


RenderSystem::RenderSystem(std::unique_ptr<IRenderer> renderer)
    : renderer_(std::move(renderer)) {
    if (!renderer_) {
        throw std::invalid_argument("RenderSystem: renderer cannot be null");
    }
}

void RenderSystem::render(GameController& controller) {
    // Clear screen for new frame
    renderer_->clear();

    // Get references to world and player
    World* world = controller.getWorld();
    Player* player = controller.getPlayer();

    if (!world || !player) {
        renderer_->present();
        return;
    }

    // Render the active map and player
    // NOTE: World needs a getActiveMap() method for this to work.
    // For now, this assumes Map is accessible from World.
    // Adjust based on your actual World interface.
    
    // TODO: Add getActiveMap() to World class to access current map
    // For now, we'll render what we can through the World interface
    // world->render();

    // Draw the player on top of the map
    renderer_->drawPlayer(player->getX(), player->getY(), player->getDirection());

    // Present the frame
    renderer_->present();
}
