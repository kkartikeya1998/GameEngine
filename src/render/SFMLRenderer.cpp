#include "render/SFMLRenderer.h"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <iostream>

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
    float scale = static_cast<float>(TILE_SIZE) / region.sourceTileSize;

    sprite.setScale(sf::Vector2f(scale, scale));

    sprite.setPosition(sf::Vector2f(
        screenX(gridX),
        screenY(gridY)
    ));

    window_->draw(sprite);
}

void SFMLRenderer::drawMapObject(float originPixelX, float originPixelY, const std::string& typeName) {
    SpriteRegion region = mapObjAtlas_.getObjectSprite(typeName);

    // Each object type can point at its own texture (or share one with
    // other types) — region.texture is set by MapObjectAtlas per lookup.
    // Falling back to objectTexture() is defensive only
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
    float scale = TILE_SIZE / sourceTileSize;
    // float scaleY = TILE_SIZE / sourceTileSize;

    sprite.setScale(sf::Vector2f(scale, scale));

    float spriteWidth = region.tile_size.x * scale;
    float spriteHeight = region.tile_size.y * scale;


    // float anchorX = static_cast<float>(gridX);
    // float anchorY = static_cast<float>(gridY);

    sprite.setPosition(sf::Vector2f(
        originPixelX - spriteWidth / 2.f,
        originPixelY - spriteHeight
    ));

    window_->draw(sprite);
}


void SFMLRenderer::drawDebugRect(float x, float y, float width, float height) {
    // Innermost ring (margin 0, the actual box) is solid red; each
    // successive ring lightens toward white while staying in the same
    // red hue family (green/blue channels rise together, red stays
    // maxed), so they read as "the same color, fading outward" rather
    // than a hue shift.
    static constexpr float kMargins[]     = { 0.f, 5.f, 10.f, 20.f };
    static constexpr float kThicknesses[] = { 3.f, 2.f, 2.f, 1.f };
    static const sf::Color kColors[] = {
        sf::Color(255, 0,   0),    // innermost — solid red
        sf::Color(255, 80,  80),   // light red
        sf::Color(255, 150, 150),  // lighter red / pink
        sf::Color(255, 210, 210),  // lightest — near-white pink
    };

    for (std::size_t i = 0; i < std::size(kMargins); ++i) {
        float margin = kMargins[i];

        sf::RectangleShape rect(sf::Vector2f(
            width  + 2.f * margin,
            height + 2.f * margin
        ));
        rect.setPosition(sf::Vector2f(
            x - margin,
            y - margin
        ));
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(kColors[i]);
        rect.setOutlineThickness(kThicknesses[i]);

        window_->draw(rect);
    }
}

void SFMLRenderer::drawPlayer(float worldX, float worldY, Direction facing, float animProgress) {
    WalkFrame frame = SpriteAtlas::frameFromProgress(animProgress);
    SpriteRegion region = spriteAtlas_.getPlayerSprite(facing, frame);
    const sf::Texture& tex = spriteAtlas_.playerTexture();

    sf::Sprite sprite(tex);
    sprite.setTextureRect(region.subrect);

    
    float sourceTileSize = static_cast<float>(region.sourceTileSize);
    float scale = TILE_SIZE / sourceTileSize;

    sprite.setScale(sf::Vector2f(scale, scale));

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
    // drawDirectionIndicator(drawX, drawY, facing);
}
