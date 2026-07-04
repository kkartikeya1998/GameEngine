#pragma once

#include <memory>
#include <optional>
#include <SFML/Window/Event.hpp>

#include "render/IRenderer.h"
#include "system/GameController.h"

// ---------------------------------------------------------------------------
// RenderSystem.h
// ---------------------------------------------------------------------------
class RenderSystem {
public:
    explicit RenderSystem(std::unique_ptr<IRenderer> renderer);

    void render(GameController& controller, float dt);

    std::optional<sf::Event> pollEvent();

    bool isOpen() const;

private:
    std::unique_ptr<IRenderer> renderer_;
};