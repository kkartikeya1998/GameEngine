#include "render/RenderSystem.h"
#include "system/GameConstants.h"
#include <algorithm>
#include <iostream>

RenderSystem::RenderSystem(std::unique_ptr<IRenderer> renderer,
                           const TileRepository &tileRepository,
                           const std::filesystem::path &tileSpritesheetPath)
    : renderer_(std::move(renderer)), tileAtlas_(tileSpritesheetPath, tileRepository), tileTexturePath_(tileSpritesheetPath.string())
{
    if (!renderer_)
        throw std::invalid_argument("RenderSystem: renderer cannot be null");
}

void RenderSystem::beginFrame(const Camera &camera)
{
    currentCamera_ = camera;
    queue_.clear();
    debugQueue_.clear();
}

void RenderSystem::submit(RenderLayer layer, float z,
                          RenderComponent render, RenderAnchor anchor)
{
    queue_.push_back(Renderable{
        layer,
        z,
        false,
        [this, render, anchor]()
        {
            renderer_->drawEntity(render, anchor);
        }});
}

void RenderSystem::submitDebugRect(float x, float y, float width, float height)
{
    debugQueue_.push_back(Renderable{
        RenderLayer::ScreenOverlay,
        0.f,
        false,
        [this, x, y, width, height]()
        {
            renderer_->drawDebugRect(x, y, width, height);
        }});
}

void RenderSystem::submitRect(RenderLayer layer, float z, float x, float y,
                              float width, float height, sf::Color color,
                              bool screenSpace)
{
    queue_.push_back(Renderable{
        layer,
        z,
        screenSpace,
        [this, x, y, width, height, color, screenSpace]()
        {
            renderer_->drawRect(x, y, width, height, color, screenSpace);
        }});
}

void RenderSystem::submitTile(int gridX, int gridY,
                              const RenderComponent &tileRender)
{
    SpriteRegion region = tileAtlas_.getTileSprite(tileRender.name);

    RenderComponent resolved = tileRender;
    resolved.texturePath = tileTexturePath_;
    resolved.textureRect = sf::IntRect(region.subrect);
    resolved.sourceTileSize = region.sourceTileSize;
    resolved.renderX = static_cast<float>(gridX) * GameConstants::TILE_SIZE;
    resolved.renderY = static_cast<float>(gridY) * GameConstants::TILE_SIZE;

    submit(resolved.layer, resolved.z, resolved, RenderAnchor::TopLeft);
}

void RenderSystem::submitText(RenderLayer layer, float z,
                              const sf::Font &font,
                              const std::string &text,
                              float x, float y,
                              unsigned int characterSize,
                              sf::Color color,
                              bool screenSpace)
{

    // std::cout << "[RenderSystem] submitText entered.\n";
    queue_.push_back(Renderable{
        layer,
        z,
        screenSpace,
        [this, font, text, x, y, characterSize, color, screenSpace]()
        {
            sf::Text sfText(font); // SFML 3.x requires font in ctor
            sfText.setString(text);
            sfText.setCharacterSize(characterSize);
            sfText.setFillColor(color);
            sfText.setPosition(sf::Vector2f{x, y});

            renderer_->drawText(sfText);
        }});
        
    // std::cout << "[RenderSystem] submitText exiting...\n";
}

void RenderSystem::endFrame()
{
    // std::cout << "[RenderSystem] endFrame entered.\n";
    renderer_->clear();

    // Sort by layer/z
    std::stable_sort(queue_.begin(), queue_.end(),
                     [](const Renderable &a, const Renderable &b)
                     {
                         return a.layer != b.layer ? a.layer < b.layer : a.z < b.z;
                     });

    // 1. World-space pass (entities + debug), camera view.
    renderer_->beginWorldView(currentCamera_);

    for (auto &r : queue_)
    {
        if (!r.screenSpace)
        {
            r.draw();
        }
    }

    for (auto &r : debugQueue_)
    {
        r.draw(); // debug is world-space
    }

    // 2. Screen-space pass (UI overlay), default view.
    renderer_->setDefaultView();

    for (auto &r : queue_)
    {
        if (r.screenSpace)
        {
            r.draw();
        }
    }

    renderer_->present();
    // std::cout << "[RenderSystem] endFrame exiting.\n";
}

bool RenderSystem::isOpen() const
{
    return renderer_->isOpen();
}

std::optional<sf::Event> RenderSystem::pollEvent()
{
    return renderer_->pollEvent();
}