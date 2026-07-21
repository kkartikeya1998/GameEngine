#include "render/SFMLRenderer.h"
#include "exceptions/EngineExceptions.h"
#include "log/Logger.h"
#include <SFML/Graphics.hpp>
#include <format>

SFMLRenderer::SFMLRenderer(int windowWidth, int windowHeight)
{
    (void)windowWidth;
    (void)windowHeight;

    LOG_INFO("Initializing renderer");
    window_ = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT), 32),
        "Pokemon Game");

    if (!window_->isOpen())
    {
        // Unrecoverable: no display, driver failure, headless environment,
        // etc. — there's nothing sensible to render into. Thrown with full
        // context in the message and NOT logged here too; this propagates
        // through Game's constructor to main()'s single top-level
        // boundary, which logs it once (see main.cpp) rather than twice.
        throw RendererInitException(
            std::format("SFMLRenderer: failed to create window ({}x{})", WINDOW_WIDTH, WINDOW_HEIGHT));
    }

    window_->setFramerateLimit(GameConstants::FRAME_RATE);
    LOG_INFO(std::format("SFML Renderer initialized: {}x{}", WINDOW_WIDTH, WINDOW_HEIGHT));
}

SFMLRenderer::~SFMLRenderer() = default;

void SFMLRenderer::clear()
{
    LOG_TRACE("Clearing window");
    window_->clear(sf::Color::Black);
}

void SFMLRenderer::beginWorldView(const Camera &camera)
{
    LOG_TRACE("Setting world view");
    window_->setView(sf::View(
        sf::Vector2f(camera.centerX, camera.centerY),
        sf::Vector2f(camera.viewWidth, camera.viewHeight)));
}

void SFMLRenderer::present()
{
    LOG_TRACE("Presenting window");
    window_->display();
}

bool SFMLRenderer::isOpen() const
{
    return window_ && window_->isOpen();
}

std::unique_ptr<sf::Texture> SFMLRenderer::buildMissingTexture()
{
    LOG_WARNING("Procedurally generated missing texture");
    // The traditional magenta/black "missing texture" checkerboard —
    // visible enough in-game to get noticed and reported, without taking
    // the frame down over one bad asset reference.
    constexpr unsigned kSize = 32;
    sf::Image image({kSize, kSize}, sf::Color::Black);
    for (unsigned y = 0; y < kSize; ++y)
        for (unsigned x = 0; x < kSize; ++x)
            if (((x / 8) + (y / 8)) % 2 == 0)
                image.setPixel({x, y}, sf::Color::Magenta);

    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromImage(image))
    {
        // If even a procedurally-generated in-memory texture fails to
        // load, the graphics context itself is broken — that's no longer
        // a content problem, it's a renderer failure.
        throw RendererInitException("SFMLRenderer: failed to create fallback placeholder texture");
    }
    return tex;
}

const sf::Texture &SFMLRenderer::getOrLoadTexture(const std::string &path)
{
    LOG_TRACE("Fetching texture: " + path);
    auto it = textureCache_.find(path);
    if (it != textureCache_.end())
        return *it->second;

    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromFile(path))
    {
        // Recoverable: a bad/missing texture path is content data, not an
        // engine invariant. Log once per unique path (see warnedMissingTextures_
        // on why), then draw a placeholder instead of throwing — a wrong-
        // looking sprite is far better than losing the whole session over
        // one bad reference.
        if (warnedMissingTextures_.insert(path).second)
            LOG_ERROR(std::format("Failed to load texture: {} — using placeholder", path));

        if (!missingTexture_)
            missingTexture_ = buildMissingTexture();

        // Cache under the requested path too, so repeated draws of the
        // same broken reference hit the cache instead of retrying the
        // failed disk read on every single call.
        auto [inserted, _] = textureCache_.emplace(path, std::make_unique<sf::Texture>(*missingTexture_));
        return *inserted->second;
    }

    LOG_DEBUG(std::format("Loaded texture: {}", path));
    auto [inserted, _] = textureCache_.emplace(path, std::move(tex));
    return *inserted->second;
}

void SFMLRenderer::drawEntity(const ResolvedSprite &sprite, RenderAnchor anchor)
{
    const sf::Texture &tex = getOrLoadTexture(sprite.texturePath);

    sf::Sprite sfSprite(tex);
    sfSprite.setTextureRect(sprite.textureRect);

    const float scale = sprite.renderScale;
    sfSprite.setScale(sf::Vector2f(scale, scale));

    const float width = sprite.textureRect.size.x * scale;
    const float height = sprite.textureRect.size.y * scale;

    sf::Vector2f pos = (anchor == RenderAnchor::TopLeft)
                           ? sf::Vector2f(sprite.x, sprite.y)
                           : sf::Vector2f(sprite.x - width * 0.5f, sprite.y - height);

    sfSprite.setPosition(pos);
    window_->draw(sfSprite);
}

void SFMLRenderer::drawDebugRect(float x, float y, float width, float height)
{
    static constexpr float kMargins[] = {0.f, 5.f, 10.f, 20.f};
    static constexpr float kThicknesses[] = {3.f, 2.f, 2.f, 1.f};
    static const sf::Color kColors[] = {
        sf::Color(255, 0, 0),
        sf::Color(255, 80, 80),
        sf::Color(255, 150, 150),
        sf::Color(255, 210, 210),
    };

    for (std::size_t i = 0; i < std::size(kMargins) - 2; ++i)
    {
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

void SFMLRenderer::drawText(const sf::Text &text)
{
    window_->draw(text);
}

void SFMLRenderer::setDefaultView()
{
    window_->setView(window_->getDefaultView());
}

std::optional<sf::Event> SFMLRenderer::pollEvent()
{
    return window_->pollEvent();
}