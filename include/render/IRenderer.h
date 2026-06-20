#pragma once

#include <optional>
#include <SFML/Window/Event.hpp>
#include "world/Terrain.h"
#include "entities/movement/Position.h"

// ---------------------------------------------------------------------------
// IRenderer — abstract interface for rendering.
//
// The renderer doesn't know or care about World, Player, or GameController.
// It only knows how to draw primitive elements: tiles, players, and UI.
//
// Responsibility:
//   - Draw game elements on the target backend (SFML, console, web, etc.)
//   - Manage the render window/display
//   - Be platform-agnostic from the game logic's perspective
// ---------------------------------------------------------------------------
class IRenderer {
public:
    virtual ~IRenderer() = default;

    // Clear the screen for a new frame
    virtual void clear() = 0;

    // Draw a single tile at grid position (x, y)
    virtual void drawTile(int gridX, int gridY, Terrain::Type terrain) = 0;

    // Entities can be drawn at fractional grid positions for smooth animation
    virtual void drawPlayer(float gridX, float gridY, Direction facing, float animProgress) = 0;

    // Present the rendered frame to the screen
    virtual void present() = 0;

    // Check if the window should close
    virtual bool isOpen() const = 0;

    // Handle input events (keyboard, window close, etc.)
    // Returns true if a quit event was triggered
    virtual std::optional<sf::Event> pollEvent() = 0;
};