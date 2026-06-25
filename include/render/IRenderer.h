#pragma once

#include <optional>
#include <string>
#include <SFML/Window/Event.hpp>
#include "entities/movement/Position.h"

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

    // Draw a single tile at grid position (x, y). typeName is the tile's
    // catalog key (e.g. "Grass2", "Sand") — used to look up its sprite
    // region directly, the same way drawMapObject already does for
    // object type names. CHANGED from Terrain::Type: the renderer no
    // longer needs to know about Terrain::Type at all, since rendering
    // and gameplay-terrain-category are now separate concerns (see
    // Tile.h for why).
    virtual void drawTile(int gridX, int gridY, const std::string& typeName) = 0;

    // Entities can be drawn at fractional grid positions for smooth animation
    virtual void drawPlayer(float worldX, float worldY, Direction facing, float animProgress) = 0;

    // Draw a single placed map object (tree, house, sign, ...) at its
    // origin grid position. typeName is the object's catalog key (e.g.
    // "tree", "house") — used to look up its sprite region, the same way
    // drawTile uses Terrain::Type to look up a tile's sprite region.
    virtual void drawMapObject(float originPixelX, float originPixelY, const std::string& typeName) = 0;

    // Present the rendered frame to the screen
    virtual void present() = 0;

    // Check if the window should close
    virtual bool isOpen() const = 0;

    // Handle input events (keyboard, window close, etc.)
    // Returns true if a quit event was triggered
    virtual std::optional<sf::Event> pollEvent() = 0;
};