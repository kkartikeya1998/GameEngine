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
    Entity* player = controller.getPlayer();

    if (!world || !player) {
        renderer_->present();
        return;
    }

    // -------------------------
    // Draw map
    // -------------------------
    Map* map = world->getActiveMap();

    if (map) {
        for (int y = 0; y < map->getHeight(); y++) {
            for (int x = 0; x < map->getWidth(); x++) {
                renderer_->drawTile(
                    x,
                    y,
                    map->tile_at(x, y).terrain()
                );
            }
        }
    }

    // -------------------------
    // Draw player (animated position)
    // -------------------------
    renderer_->drawPlayer(
        player->getRenderX(),
        player->getRenderY(),
        player->getDirection(),
        player->getAnimationProgress()
    );

    // Present the frame
    renderer_->present();
}

bool RenderSystem::isOpen() const {
    return renderer_->isOpen();
}
std::optional<sf::Event> RenderSystem::pollEvent(){
    return renderer_->pollEvent();
}