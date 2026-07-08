#pragma once

#include <memory>
#include <optional>
#include <SFML/Window/Event.hpp>

#include "render/IRenderer.h"
#include "render/Camera.h"
#include "render/atlases/TileAtlas.h"
#include "asset/repositories/TileRepository.h"
#include "system/GameController.h"

// ---------------------------------------------------------------------------
// RenderSystem.h — owns the repository-facing lookup step; the renderer
// only ever sees fully-resolved RenderComponents.
// ---------------------------------------------------------------------------
class RenderSystem {
public:
    RenderSystem(std::unique_ptr<IRenderer> renderer,
                 const TileRepository& tileRepository,
                 const std::filesystem::path& tileSpritesheetPath);

    void render(GameController& controller, const Camera& camera, float dt);

    std::optional<sf::Event> pollEvent();
    bool isOpen() const;

private:
    std::unique_ptr<IRenderer> renderer_;
    TileAtlas tileAtlas_;
    std::string tileTexturePath_; // TODO: confirm TileRepository metadata doesn't already carry this

    // Builds a fully-resolved RenderComponent for a tile at grid (x, y)
    RenderComponent resolveTile(int gridX, int gridY, const RenderComponent& tileRender) const;
};