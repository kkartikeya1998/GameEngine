#include "render/RenderSystem.h"
#include "system/GameConstants.h"
#include <algorithm>

RenderSystem::RenderSystem(std::unique_ptr<IRenderer> renderer,
                            const TileRepository& tileRepository,
                            const std::filesystem::path& tileSpritesheetPath)
    : renderer_(std::move(renderer))
    , tileAtlas_(tileSpritesheetPath, tileRepository)
    , tileTexturePath_(tileSpritesheetPath.string())
{
    if (!renderer_)
        throw std::invalid_argument("RenderSystem: renderer cannot be null");
}

void RenderSystem::beginFrame(const Camera& camera)
{
    currentCamera_ = camera;
    queue_.clear();
    debugQueue_.clear();
}

void RenderSystem::submit(RenderLayer layer, float z, RenderComponent render, RenderAnchor anchor)
{
    queue_.push_back(Renderable{
        layer,
        z,
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
        [this, x, y, width, height]()
        {
            renderer_->drawDebugRect(x, y, width, height);
        }});
}

void RenderSystem::submitRect(RenderLayer layer, float z, float x, float y,
                               float width, float height, sf::Color color, bool screenSpace)
{
    queue_.push_back(Renderable{
        layer,
        z,
        [this, x, y, width, height, color, screenSpace]()
        {
            renderer_->drawRect(x, y, width, height, color, screenSpace);
        }});
}

void RenderSystem::submitTile(int gridX, int gridY, const RenderComponent& tileRender)
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

void RenderSystem::endFrame()
{
    renderer_->clear();
    renderer_->beginWorldView(currentCamera_);

    std::stable_sort(queue_.begin(), queue_.end(),
              [](const Renderable& a, const Renderable& b)
              {
                  return a.layer != b.layer ? a.layer < b.layer : a.z < b.z;
              });

    for (auto& r : queue_)
        r.draw();

    for (auto& r : debugQueue_)
        r.draw();

    renderer_->present();
}

bool RenderSystem::isOpen() const
{
    return renderer_->isOpen();
}

std::optional<sf::Event> RenderSystem::pollEvent()
{
    return renderer_->pollEvent();
}