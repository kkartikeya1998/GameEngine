#pragma once

#include <memory>
#include <optional>
#include <SFML/Window/Event.hpp>

#include "render/IRenderer.h"
#include "system/GameController.h"

// ---------------------------------------------------------------------------
// RenderSystem.h — NOT independently shared with me; this is
// reconstructed from RenderSystem.cpp's existing method bodies and
// Game.h/Game.cpp's usage (renderSystem_->isOpen(),
// renderSystem_->pollEvent(), renderSystem_->render(controller_)).
//
// ONLY CHANGE versus whatever your real header currently has: render()
// gains a `float dt` parameter — required because RenderSystem now
// steps each entity's render-state component (lerp/walk-cycle) inline
// during render() rather than during an update phase (see the answer
// to "where should render-state stepping live" — folded into render()
// itself, which means render() now needs dt to do that stepping).
//
// If your real RenderSystem.h has additional members/methods beyond
// what's inferable from the .cpp and Game.cpp, reconcile this against
// that — I'm not guessing at anything beyond the one signature change
// this redesign requires.
// ---------------------------------------------------------------------------
class RenderSystem {
public:
    explicit RenderSystem(std::unique_ptr<IRenderer> renderer);

    void render(GameController& controller, float dt);

    bool isOpen() const;
    std::optional<sf::Event> pollEvent();

private:
    std::unique_ptr<IRenderer> renderer_;
};