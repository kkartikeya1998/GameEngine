#pragma once

#include <memory>

#include "render/IRenderer.h"
#include "system/GameController.h"

// // Forward declarations
// class GameController;
// class World;
// class Map;
// class Player;

// ---------------------------------------------------------------------------
// RenderSystem — orchestrates the rendering of the game state.
//
// Responsibility:
//   - Queries GameController for game state
//   - Issues drawing commands to IRenderer in the correct order
//   - Separates the "what to render" from the "how to render it"
//
// This system depends on IRenderer (abstraction), not on any concrete
// renderer. Can easily swap SFML for console, SDL, web, etc.
// ---------------------------------------------------------------------------
class RenderSystem {
public:
    explicit RenderSystem(std::unique_ptr<IRenderer> renderer);

    // Render the complete game state from GameController
    void render(GameController& controller);

    bool isOpen() const;
    std::optional<sf::Event> pollEvent();

    

private:
    std::unique_ptr<IRenderer> renderer_;

    // Helper methods
    void renderMap(Map& map, Player& player);
};