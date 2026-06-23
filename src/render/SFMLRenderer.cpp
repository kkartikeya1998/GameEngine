#include "render/SFMLRenderer.h"
#include <SFML/Graphics.hpp>
#include <stdexcept>

SFMLRenderer::SFMLRenderer(int windowWidth, int windowHeight,
                            const MapRepository& tileRepository,
                            const MapObjectRepository& objectRepository,
                            const SpriteRepository& spriteRepository,
                            const std::string& tileSpritesheetPath,
                            const std::string& playerSpritesheetPath)
    : tileAtlas_(tileSpritesheetPath, tileRepository)
    , spriteAtlas_(playerSpritesheetPath, spriteRepository)
    , mapObjAtlas_(objectRepository)
{
    (void)windowWidth;
    (void)windowHeight;

    window_ = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT), 32),
        "Pokemon Game"
    );
    window_->setFramerateLimit(60);
    initTerrainColors();
}

SFMLRenderer::~SFMLRenderer() = default;

void SFMLRenderer::initTerrainColors() {
    terrainColors_[Terrain::Type::Grass]    = {34, 139, 34};
    terrainColors_[Terrain::Type::Sand]     = {210, 180, 140};
    terrainColors_[Terrain::Type::Water]    = {30, 144, 255};
    terrainColors_[Terrain::Type::Ice]      = {173, 216, 230};
    terrainColors_[Terrain::Type::Cave]     = {105, 105, 105};
    terrainColors_[Terrain::Type::Building] = {128, 128, 128};
    terrainColors_[Terrain::Type::Tree]     = {0, 100, 0};
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

void SFMLRenderer::drawTile(int gridX, int gridY, Terrain::Type terrain) {
    SpriteRegion region = tileAtlas_.getTerrainSprite(terrain);
    const sf::Texture& tex = tileAtlas_.terrainTexture();

    sf::Sprite sprite(tex);
    sprite.setTextureRect(region.subrect);

    sprite.setScale(sf::Vector2f(
        TILE_SIZE / region.tile_size.x,
        TILE_SIZE / region.tile_size.y
    ));

    sprite.setPosition(sf::Vector2f(
        screenX(gridX),
        screenY(gridY)
    ));

    window_->draw(sprite);
}

void SFMLRenderer::drawPlayer(float gridX, float gridY, Direction facing, float animProgress) {
    WalkFrame frame = SpriteAtlas::frameFromProgress(animProgress);
    SpriteRegion region = spriteAtlas_.getPlayerSprite(facing, frame);
    const sf::Texture& tex = spriteAtlas_.playerTexture();

    sf::Sprite sprite(tex);
    sprite.setTextureRect(region.subrect);

    sprite.setScale(sf::Vector2f(
        TILE_SIZE / region.tile_size.x,
        TILE_SIZE / region.tile_size.y
    ));

    sprite.setPosition(sf::Vector2f(
        screenX(gridX),
        screenY(gridY) - 8.f
    ));

    window_->draw(sprite);
    drawDirectionIndicator(screenX(gridX), screenY(gridY), facing);
}

void SFMLRenderer::drawDirectionIndicator(float screenX, float screenY, Direction facing) {
    sf::ConvexShape arrow(3);
    arrow.setFillColor(sf::Color::White);

    float centerX = screenX + TILE_SIZE / 2.f;
    float centerY = screenY + TILE_SIZE / 2.f;
    float offset = 6.f;

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