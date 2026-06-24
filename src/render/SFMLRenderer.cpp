#include "render/SFMLRenderer.h"
#include <SFML/Graphics.hpp>
#include <stdexcept>

SFMLRenderer::SFMLRenderer(int windowWidth, int windowHeight,
                            const TileRepository& tileRepository,
                            const MapObjectRepository& objectRepository,
                            const SpriteRepository& spriteRepository,
                            const std::string& tileSpritesheetPath,
                            const std::string& playerSpritesheetPath,
                            const std::string& objectSpritesheetPath)
    : tileAtlas_(tileSpritesheetPath, tileRepository)
    , spriteAtlas_(playerSpritesheetPath, spriteRepository)
    , mapObjAtlas_(objectSpritesheetPath, objectRepository)
{
    (void)windowWidth;
    (void)windowHeight;

    window_ = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT), 32),
        "Pokemon Game"
    );
    window_->setFramerateLimit(60);
    // initTerrainColors();
}

SFMLRenderer::~SFMLRenderer() = default;

void SFMLRenderer::initTerrainColors() {
    // terrainColors_[Terrain::Type::Grass1]    = {34, 139, 34};
    // terrainColors_[Terrain::Type::Sand]     = {210, 180, 140};
    // terrainColors_[Terrain::Type::Water]    = {30, 144, 255};
    // terrainColors_[Terrain::Type::Ice]      = {173, 216, 230};
    // terrainColors_[Terrain::Type::Cave]     = {105, 105, 105};
    // terrainColors_[Terrain::Type::Building] = {128, 128, 128};
    // terrainColors_[Terrain::Type::Tree]     = {0, 100, 0};
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


void SFMLRenderer::drawMapObject(int gridX, int gridY, const std::string& typeName) {
    SpriteRegion region = mapObjAtlas_.getObjectSprite(typeName);

    // Each object type can point at its own texture (or share one with
    // other types) — region.texture is set by MapObjectAtlas per lookup.
    // Falling back to objectTexture() is defensive only; getObjectSprite
    // always sets region.texture today.
    const sf::Texture& tex = region.texture ? *region.texture : mapObjAtlas_.objectTexture();

    sf::Sprite sprite(tex);
    sprite.setTextureRect(region.subrect);

    // Scale BOTH axes independently so the sprite renders at its true
    // size in tiles, derived from its actual pixel dimensions relative to
    // TILE_SIZE — not forced into a single 1x1 tile box. A 96x32 House
    // (3 tiles wide, 1 tile tall) renders 3 tiles wide; a 32x64 Tree
    // (1 tile wide, 2 tiles tall) renders 1 tile wide, 2 tiles tall. Each
    // axis scales independently because source art width and height
    // aren't proportional to each other in general.
    //
    // NOTE: this assumes source sprites are authored at a consistent
    // pixels-per-tile unit across the spritesheet (w/h here are
    // multiples of 32, while TILE_SIZE is 64 — source art is at half
    // TILE_SIZE's resolution). If that ratio isn't constant across your
    // spritesheet, this would need to come from metadata instead of
    // being inferred as a single constant.
    constexpr float SOURCE_PIXELS_PER_TILE = 32.f;
    float scaleX = TILE_SIZE / SOURCE_PIXELS_PER_TILE;
    float scaleY = TILE_SIZE / SOURCE_PIXELS_PER_TILE;

    sprite.setScale(sf::Vector2f(scaleX, scaleY));

    float spriteWidth = region.tile_size.x * scaleX;
    float spriteHeight = region.tile_size.y * scaleY;

    // Anchor: horizontally centered on the origin tile (a 3-tile-wide
    // House, placed with origin at its middle/door tile per the
    // footprint, ends up centered rather than growing off to one side);
    // vertically anchored at the BOTTOM of the origin tile, so anything
    // taller than 1 tile (Tree, Boulder) grows upward instead of sinking
    // below ground — same idea as drawPlayer's "- 8.f" foot-planting
    // offset, generalized to any height.
    sprite.setPosition(sf::Vector2f(
        screenX(gridX) + TILE_SIZE / 2.f - spriteWidth / 2.f,
        screenY(gridY) + TILE_SIZE - spriteHeight
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