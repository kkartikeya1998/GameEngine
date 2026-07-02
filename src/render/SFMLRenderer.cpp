#include "render/SFMLRenderer.h"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <iostream>

SFMLRenderer::SFMLRenderer(int windowWidth, int windowHeight,
                            const TileRepository& tileRepository,
                            const MapObjectRepository& objectRepository,
                            const SpriteRepository& spriteRepository,
                            const std::filesystem::path& tileSpritesheetPath,
                            const std::filesystem::path& playerSpritesheetPath,
                            const std::filesystem::path& objectSpritesheetPath)
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
    window_->setFramerateLimit(GameConstants::FRAME_RATE);
}

SFMLRenderer::~SFMLRenderer() = default;

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
void SFMLRenderer::drawMapObject(const PositionComponent& objectPos,
                                 const MapObjectRenderComponent& objectRender)
{
    const auto& metadata = *objectRender.metadata;

    const sf::Texture& texture = mapObjAtlas_.resolveTexture(metadata.texturePath);
    sf::Sprite sprite(texture);
    sprite.setTextureRect(metadata.textureRect);

    const float sourceTileSize = static_cast<float>(metadata.sourceTileSize);
    const float scale          = TILE_SIZE / sourceTileSize;
    sprite.setScale({scale, scale});

    const float spriteWidth  = metadata.textureRect.size.x * scale;
    const float spriteHeight = metadata.textureRect.size.y * scale;

    sprite.setPosition({
        objectPos.x - spriteWidth * 0.5f,
        objectPos.y - spriteHeight
    });

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

void SFMLRenderer::drawPlayer(const PositionComponent& playerPos, const DirectionComponent& playerDir, float animProgress) {
    WalkFrame frame = SpriteAtlas::frameFromProgress(animProgress);
    SpriteRegion region = spriteAtlas_.getPlayerSprite(playerDir.facing, frame);
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
    float drawX = playerPos.x - spriteWidth / 2.f;
    float drawY = playerPos.y - spriteHeight;

    sprite.setPosition(sf::Vector2f(drawX, drawY));

    window_->draw(sprite);
    // drawDirectionIndicator(drawX, drawY, facing);
}
