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

void SFMLRenderer::drawTile(int gridX, int gridY, const std::string& typeName) {
    SpriteRegion region = tileAtlas_.getTerrainSprite(typeName);
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

    // Scale BOTH axes independently using this object's own
    // sourceTileSize (pixels-per-tile in ITS source art), read from
    // metadata rather than assumed as a fixed constant. This makes the
    // scale factor resolution-independent: a tileset re-exported at
    // 128px/tile, or an object sheet at some other density, just needs
    // its JSON's "tile_size" updated — no renderer code change. A 96x32
    // House at sourceTileSize 32 renders 3 tiles wide; the same House
    // re-exported at 96x96px with sourceTileSize 32 would still render
    // 3 tiles wide, just with 3x the source detail per tile.
    float sourceTileSize = static_cast<float>(region.sourceTileSize);
    float scaleX = TILE_SIZE / sourceTileSize;
    float scaleY = TILE_SIZE / sourceTileSize;

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
        // screenX(gridX) + TILE_SIZE / 2.f - spriteWidth / 2.f,
        // screenY(gridY) + TILE_SIZE - spriteHeight
        gridX, gridY
    ));

    window_->draw(sprite);
}



void SFMLRenderer::drawPlayer(float worldX, float worldY, Direction facing, float animProgress) {
    WalkFrame frame = SpriteAtlas::frameFromProgress(animProgress);
    SpriteRegion region = spriteAtlas_.getPlayerSprite(facing, frame);
    const sf::Texture& tex = spriteAtlas_.playerTexture();

    sf::Sprite sprite(tex);
    sprite.setTextureRect(region.subrect);

    sprite.setScale(sf::Vector2f(
        TILE_SIZE / region.tile_size.x,
        TILE_SIZE / region.tile_size.y
    ));

    // Use the sprite's OWN scaled bounding box for width/height, instead
    // of re-deriving it from region.tile_size + an aspect-ratio formula.
    // getGlobalBounds() reflects setScale()'s actual effect directly —
    // one less place for a height-calculation mismatch to creep in.
    sf::FloatRect bounds = sprite.getGlobalBounds();
    float spriteWidth = bounds.size.x;
    float spriteHeight = bounds.size.y;

    // worldX/worldY are the player's bottom-center point — this matches
    // FreeMovementMechanics::hitboxAt's anchor convention exactly
    // (hitboxAt computes the box as x - width/2, y - height, meaning
    // position_.y IS the bottom edge / "feet" position). The sprite's
    // top-left corner needs to land at (worldX - half its width,
    // worldY - its full height) to make the visible sprite line up with
    // where collision is actually computed, instead of the sprite's
    // top-left sitting directly on the feet position.
    float drawX = worldX - spriteWidth / 2.f;
    float drawY = worldY - spriteHeight;

    sprite.setPosition(sf::Vector2f(drawX, drawY));

    window_->draw(sprite);
    drawDirectionIndicator(drawX, drawY, facing);
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