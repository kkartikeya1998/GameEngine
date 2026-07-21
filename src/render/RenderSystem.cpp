#include "render/RenderSystem.h"
#include "system/GameConstants.h"
#include "exceptions/Assert.h"
#include "log/Logger.h"
#include <algorithm>
#include <format>

RenderSystem::RenderSystem(std::unique_ptr<IRenderer> renderer,
                           const ComponentAssetRepository<RenderAssetMetadata> &renderRepository,
                           const std::filesystem::path &tileSpritesheetPath)
    : renderer_(std::move(renderer)), tileAtlas_(tileSpritesheetPath, renderRepository), tileTexturePath_(tileSpritesheetPath.string())
{
    // A null renderer here means the one call site that constructs
    // RenderSystem (Game.cpp) was changed to pass something other than a
    // freshly-made_unique concrete renderer — a wiring/logic bug, not a
    // condition that can arise from bad data or a bad frame at runtime.
    // That's an assertion, not an exception (exceptions/Assert.h): it fails
    // loudly and immediately in every build rather than surfacing later
    // as a null-deref somewhere in endFrame().
    ENGINE_ASSERT_MSG(renderer_ != nullptr, "RenderSystem constructed with a null renderer");
}

void RenderSystem::beginFrame(const Camera &camera)
{
    currentCamera_ = camera;
    queue_.clear();
    debugQueue_.clear();
}

void RenderSystem::submit(RenderLayer layer, float z, ResolvedSprite sprite, RenderAnchor anchor)
{
    queue_.push_back(Renderable{layer, z, false, SpriteDraw{std::move(sprite), anchor}});
}

void RenderSystem::submitDebugRect(float x, float y, float width, float height)
{
    debugQueue_.push_back(DebugRectDraw{x, y, width, height});
}

void RenderSystem::submitRect(RenderLayer layer, float z, float x, float y,
                              float width, float height, sf::Color color, bool screenSpace)
{
    queue_.push_back(Renderable{layer, z, screenSpace, RectDraw{x, y, width, height, color}});
}

void RenderSystem::submitTile(int gridX, int gridY, const SpriteAssetComponent &tileAsset, const SpriteFrameComponent &tileFrame)
{
    Result<SpriteRegion, AssetError> result = tileAtlas_.getTileSprite(tileFrame.name);
    if (!result)
    {
        if (warnedMissingTiles_.insert(tileFrame.name).second)
            LOG_ERROR(std::format("RenderSystem: {} — skipping tile draw", result.error().toString()));
        return;
    }

    const SpriteRegion &region = result.value();

    ResolvedSprite sprite;
    sprite.texturePath = tileTexturePath_;
    sprite.textureRect = sf::IntRect(region.subrect);
    sprite.sourceTileSize = region.sourceTileSize;
    sprite.renderScale = tileAsset.renderScale;
    sprite.x = static_cast<float>(gridX) * GameConstants::TILE_SIZE;
    sprite.y = static_cast<float>(gridY) * GameConstants::TILE_SIZE;

    submit(tileAsset.layer, tileAsset.z, sprite, RenderAnchor::TopLeft);
}

void RenderSystem::submitText(RenderLayer layer, float z,
                              const sf::Font &font, const std::string &text,
                              float x, float y, unsigned int characterSize,
                              sf::Color color, bool screenSpace)
{
    sf::Text sfText(font);
    sfText.setString(text);
    sfText.setCharacterSize(characterSize);
    sfText.setFillColor(color);
    sfText.setPosition(sf::Vector2f{x, y});

    queue_.push_back(Renderable{layer, z, screenSpace, TextDraw{std::move(sfText)}});
}

void RenderSystem::endFrame()
{
    renderer_->clear();

    std::stable_sort(queue_.begin(), queue_.end(),
                     [](const Renderable &a, const Renderable &b)
                     {
                         return a.layer != b.layer ? a.layer < b.layer : a.z < b.z;
                     });
    auto draw = [this](const Renderable &r)
    {
        std::visit([this, screenSpace = r.screenSpace](const auto &payload)
                   {
            using T = std::decay_t<decltype(payload)>;
            if constexpr (std::is_same_v<T, SpriteDraw>)
                renderer_->drawEntity(payload.sprite, payload.anchor); // IRenderer::drawEntity signature must change to take ResolvedSprite
            else if constexpr (std::is_same_v<T, RectDraw>)
                renderer_->drawRect(payload.x, payload.y, payload.width, payload.height, payload.color, screenSpace);
            else if constexpr (std::is_same_v<T, TextDraw>)
                renderer_->drawText(payload.text); }, r.payload);
    };

    renderer_->beginWorldView(currentCamera_);
    for (auto &r : queue_)
        if (!r.screenSpace)
            draw(r);
    for (auto &r : debugQueue_)
        renderer_->drawDebugRect(r.x, r.y, r.width, r.height);

    renderer_->setDefaultView();
    for (auto &r : queue_)
        if (r.screenSpace)
            draw(r);

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