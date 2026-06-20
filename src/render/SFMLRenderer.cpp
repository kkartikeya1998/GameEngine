#include "render/SFMLRenderer.h"
#include <SFML/Graphics.hpp>
#include <stdexcept>

SFMLRenderer::SFMLRenderer(int windowWidth, int windowHeight, SpriteAtlas& atlas)  : atlas_(atlas) {
    window_ = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT), 32),
        "Pokemon Game"
    );
    window_->setFramerateLimit(60);
    initTerrainColors();
}

SFMLRenderer::~SFMLRenderer() = default;

void SFMLRenderer::initTerrainColors() {
    // Define colors for each terrain type
    terrainColors_[Terrain::Type::Grass]    = {34, 139, 34};      // Forest green
    terrainColors_[Terrain::Type::Sand]     = {210, 180, 140};    // Tan
    terrainColors_[Terrain::Type::Water]    = {30, 144, 255};     // Dodger blue
    terrainColors_[Terrain::Type::Ice]      = {173, 216, 230};    // Light blue
    terrainColors_[Terrain::Type::Cave]     = {105, 105, 105};    // Dim gray
    terrainColors_[Terrain::Type::Building] = {128, 128, 128};    // Gray
    terrainColors_[Terrain::Type::Tree]     = {0, 100, 0};        // Dark green
}

void SFMLRenderer::clear() {
    window_->clear(sf::Color::Black);
}

void SFMLRenderer::present() {
    window_->display();
}

bool SFMLRenderer::isOpen() const {
    return window_ && window_->isOpen();
}

std::optional<sf::Event> SFMLRenderer::pollEvent()
{
    return window_->pollEvent();
}

// Box for a tile based on terrain type
void SFMLRenderer::drawTile(int gridX, int gridY, Terrain::Type terrain) {
    auto it = terrainColors_.find(terrain);
    if (it == terrainColors_.end()) {
        return; // Unknown terrain type
    }

    const TerrainColor& color = it->second;
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));
    tile.setPosition(sf::Vector2f(screenX(gridX), screenY(gridY)));
    tile.setFillColor(sf::Color(color.r, color.g, color.b));
    tile.setOutlineColor(sf::Color(50, 50, 50));
    tile.setOutlineThickness(1.f);

    window_->draw(tile);
}

// void SFMLRenderer::drawTile(int gridX, int gridY, Terrain::Type terrain) {
//     SpriteRegion region = atlas_.getTerrainSprite(terrain);
//     const sf::Texture& tex = atlas_.terrainTexture();

//     sf::Sprite sprite(tex);
//     sprite.setTextureRect(region.subrect);  // SFML 3: setTextureRect, not setTextureRegion

//     sprite.setScale(sf::Vector2f(
//         TILE_SIZE / region.tile_size.x,
//         TILE_SIZE / region.tile_size.y
//     ));

//     sprite.setPosition(sf::Vector2f(
//         screenX(gridX),
//         screenY(gridY)
//     ));

//     window_->draw(sprite);
// }


// void SFMLRenderer::drawPlayer(float gridX, float gridY, Direction facing) {
//     // Draw player as a colored square with a direction indicator
//     sf::RectangleShape player(sf::Vector2f(TILE_SIZE - 4, TILE_SIZE - 4));
//     player.setPosition(sf::Vector2f(screenX(gridX) + 2, screenY(gridY) + 2));
//     player.setFillColor(sf::Color::Yellow);
//     player.setOutlineColor(sf::Color::White);
//     player.setOutlineThickness(2.f);

//     window_->draw(player);

//     // Draw direction indicator (arrow)
//     drawDirectionIndicator(screenX(gridX), screenY(gridY), facing);
// }

void SFMLRenderer::drawPlayer(float gridX, float gridY, Direction facing) {
    SpriteRegion region = atlas_.getPlayerSprite(facing);
    const sf::Texture& tex = atlas_.playerTexture();

    sf::Sprite sprite(tex);
    sprite.setTextureRect(region.subrect);

    sprite.setScale(sf::Vector2f(
        TILE_SIZE / region.tile_size.x,
        TILE_SIZE / region.tile_size.y
    ));

    sprite.setPosition(sf::Vector2f(
        screenX(gridX) + 2,
        screenY(gridY) + 2
    ));

    window_->draw(sprite);

    drawDirectionIndicator(screenX(gridX), screenY(gridY), facing);
}

void SFMLRenderer::drawDirectionIndicator(int screenX, int screenY, Direction facing) {
    // Draw a small arrow/triangle pointing in the facing direction
    sf::ConvexShape arrow(3);
    arrow.setFillColor(sf::Color::White);

    int centerX = screenX + TILE_SIZE / 2;
    int centerY = screenY + TILE_SIZE / 2;
    int offset = 6;

    switch (facing) {
        case Direction::UP:
            arrow.setPoint(0, sf::Vector2f(centerX, centerY - offset));
            arrow.setPoint(1, sf::Vector2f(centerX - 3, centerY));
            arrow.setPoint(2, sf::Vector2f(centerX + 3, centerY));
            break;
        case Direction::DOWN:
            arrow.setPoint(0, sf::Vector2f(centerX, centerY + offset));
            arrow.setPoint(1, sf::Vector2f(centerX - 3, centerY));
            arrow.setPoint(2, sf::Vector2f(centerX + 3, centerY));
            break;
        case Direction::LEFT:
            arrow.setPoint(0, sf::Vector2f(centerX - offset, centerY));
            arrow.setPoint(1, sf::Vector2f(centerX, centerY - 3));
            arrow.setPoint(2, sf::Vector2f(centerX, centerY + 3));
            break;
        case Direction::RIGHT:
            arrow.setPoint(0, sf::Vector2f(centerX + offset, centerY));
            arrow.setPoint(1, sf::Vector2f(centerX, centerY - 3));
            arrow.setPoint(2, sf::Vector2f(centerX, centerY + 3));
            break;
        case Direction::NONE:
            break;
    }

    window_->draw(arrow);
}
