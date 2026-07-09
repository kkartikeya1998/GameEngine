#include "render/SFMLRenderer.h"
#include <SFML/Graphics.hpp>
#include <stdexcept>

SFMLRenderer::SFMLRenderer(int windowWidth, int windowHeight)
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

void SFMLRenderer::beginWorldView(const Camera& camera) {
    window_->setView(sf::View(
        sf::Vector2f(camera.centerX, camera.centerY),
        sf::Vector2f(camera.viewWidth, camera.viewHeight)
    ));
}

void SFMLRenderer::present() {
    window_->display();
}

bool SFMLRenderer::isOpen() const {
    return window_ && window_->isOpen();
}

const sf::Texture& SFMLRenderer::getOrLoadTexture(const std::string& path) {
    auto it = textureCache_.find(path);
    if (it != textureCache_.end())
        return *it->second;

    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromFile(path))
        throw std::runtime_error("Failed to load texture: " + path);

    auto [inserted, _] = textureCache_.emplace(path, std::move(tex));
    return *inserted->second;
}

void SFMLRenderer::drawEntity(const RenderComponent& render, RenderAnchor anchor) {
    const sf::Texture& tex = getOrLoadTexture(render.texturePath);

    sf::Sprite sprite(tex);
    sprite.setTextureRect(render.textureRect);

    const float scale = static_cast<float>(TILE_SIZE) / static_cast<float>(render.sourceTileSize);
    sprite.setScale(sf::Vector2f(scale, scale));

    const float width  = render.textureRect.size.x * scale;
    const float height = render.textureRect.size.y * scale;

    // TopLeft: renderX/renderY is the top-left corner (tiles, grid-aligned)
    // CenterBottom: renderX/renderY is the world-pixel center-bottom point
    sf::Vector2f pos = (anchor == RenderAnchor::TopLeft)
        ? sf::Vector2f(render.renderX, render.renderY)
        : sf::Vector2f(render.renderX - width * 0.5f, render.renderY - height);

    sprite.setPosition(pos);
    window_->draw(sprite);
}

void SFMLRenderer::drawDebugRect(float x, float y, float width, float height) {
    static constexpr float kMargins[]     = { 0.f, 5.f, 10.f, 20.f };
    static constexpr float kThicknesses[] = { 3.f, 2.f, 2.f, 1.f };
    static const sf::Color kColors[] = {
        sf::Color(255, 0,   0),
        sf::Color(255, 80,  80),
        sf::Color(255, 150, 150),
        sf::Color(255, 210, 210),
    };

    for (std::size_t i = 0; i < std::size(kMargins)-2; ++i) {
        float margin = kMargins[i];
        sf::RectangleShape rect(sf::Vector2f(width + 2.f * margin, height + 2.f * margin));
        rect.setPosition(sf::Vector2f(x - margin, y - margin));
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(kColors[i]);
        rect.setOutlineThickness(kThicknesses[i]);
        window_->draw(rect);
    }
}

void SFMLRenderer::drawRect(float x, float y, float width, float height,
                            sf::Color color, bool /*screenSpace*/)
{
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(sf::Vector2f(x, y));
    rect.setFillColor(color);
    window_->draw(rect);
}

void SFMLRenderer::drawText(const sf::Text& text) {
    window_->draw(text);
}

void SFMLRenderer::setDefaultView() {
    window_->setView(window_->getDefaultView());
}

std::optional<sf::Event> SFMLRenderer::pollEvent() {
    return window_->pollEvent();
}