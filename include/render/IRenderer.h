#pragma once

#include <optional>
#include <string>
#include <SFML/Window/Event.hpp>
#include "tmp/movement/DirectionComponent.h"
#include "tmp/movement/PositionComponent.h"
#include "tmp/movement/RenderComponent.h"

// ---------------------------------------------------------------------------
// IRenderer — abstract interface for rendering.
//
// The renderer doesn't know or care about World, Player, or GameController.
// It only knows how to draw primitive elements: tiles, players, map
// objects, and UI.
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

    virtual void drawTile(int gridX, int gridY, const RenderComponent& tileRender) = 0;

    // Entities can be drawn at fractional grid positions for smooth animation
    virtual void drawPlayer(const PositionComponent& playerPos, const DirectionComponent& playerDir, const RenderComponent& playerRender, float animProgress) = 0;

    virtual void drawMapObject(const RenderComponent& objRender) = 0;
    
    // Debug-only: draws an unfilled rectangle outline at world-pixel
    // coordinates (x, y = top-left), given width/height in pixels. Not
    // part of normal gameplay rendering — exists purely so collision
    // boxes, hitboxes, etc. can be visually verified during development.
    virtual void drawDebugRect(float x, float y, float width, float height) = 0;
    // Present the rendered frame to the screen
    virtual void present() = 0;

    // Check if the window should close
    virtual bool isOpen() const = 0;

    // Handle input events (keyboard, window close, etc.)
    // Returns true if a quit event was triggered
    virtual std::optional<sf::Event> pollEvent() = 0;
};