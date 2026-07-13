#pragma once

#include <memory>
#include <optional>
#include <functional>
#include <vector>
#include <SFML/Window/Event.hpp>

#include "render/IRenderer.h"
#include "render/Camera.h"
#include "render/RenderLayer.h"
#include "render/atlases/TileAtlas.h"
#include "asset/repositories/TileRepository.h"
#include "tmp/component/RenderComponent.h"
#include "asset/repositories/ComponentAssetRepository.h"
#include "asset/metadata/RenderAssetMetadata.h"

// ---------------------------------------------------------------------------
// RenderSystem.h — owns the frame's renderable queue and the layer/z sort.
// Does NOT know about World/GameController/states; callers (GameplayState,
// PauseState, ...) submit fully-formed draw requests during their Render().
// Still owns the tile-atlas lookup, since that's a repository-facing
// resolution step, not a per-state concern.
// ---------------------------------------------------------------------------
class RenderSystem
{
public:
    RenderSystem(std::unique_ptr<IRenderer> renderer,
                 const ComponentAssetRepository<RenderAssetMetadata> &renderRepository,
                 const std::filesystem::path &tileSpritesheetPath);

    // Call once per real frame, before any state's Render() runs.
    void beginFrame(const Camera &camera);

    // Queue a sprite draw. Sorted by (layer, z) at endFrame().
    void submit(RenderLayer layer, float z, RenderComponent render, RenderAnchor anchor);

    // Queue a debug rect outline (world-space). Drawn after the sorted pass,
    // same as today's debug overlay behavior.
    void submitDebugRect(float x, float y, float width, float height);

    // Queue a flat-colored rect, e.g. pause overlay. World-space by default;
    // screenSpace=true skips the camera view transform (fullscreen overlays).
    void submitRect(RenderLayer layer, float z, float x, float y,
                    float width, float height, sf::Color color, bool screenSpace = false);

    // Resolves a tile's texture rect via the atlas and submits it.
    // Kept here (not GameplayState) since it needs tileAtlas_/tileTexturePath_.
    void submitTile(int gridX, int gridY, const RenderComponent &tileRender);

    void submitText(RenderLayer layer, float z,
                    const sf::Font &font,
                    const std::string &text,
                    float x, float y,
                    unsigned int characterSize,
                    sf::Color color,
                    bool screenSpace = false);
    // Sorts the queue, draws everything, clears the queue, presents.
    void endFrame();

    std::optional<sf::Event> pollEvent();
    bool isOpen() const;

private:
    struct Renderable
    {
        RenderLayer layer;
        float z;
        bool screenSpace;
        std::function<void()> draw;
    };

    std::unique_ptr<IRenderer> renderer_;
    TileAtlas tileAtlas_;
    std::string tileTexturePath_;

    Camera currentCamera_;
    std::vector<Renderable> queue_;
    std::vector<Renderable> debugQueue_; // rects drawn after the sorted pass
};