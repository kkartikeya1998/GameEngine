#pragma once

#include <memory>
#include <map>

#include "render/IRenderer.h"

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
}

// ---------------------------------------------------------------------------
// SFMLRenderer — SFML-based concrete implementation of IRenderer.
//
// Renders the game using SFML's 2D graphics API.
// Handles window management, texture loading, and sprite drawing.
//
// Design:
//   - Tiles are rendered as colored rectangles (placeholder)
//   - Player is rendered as a colored square with direction indicator
//   - All tile sizes are 32×32 pixels
//   - All assets are dummy paths (replace with real asset paths)
// ---------------------------------------------------------------------------
class SFMLRenderer : public IRenderer {
public:
    SFMLRenderer(int windowWidth = 1024, int windowHeight = 768);
    ~SFMLRenderer() override;

    void clear() override;
    void drawTile(int gridX, int gridY, Terrain::Type terrain) override;
    // float since player position can be animated (interpolated between tiles)
    void drawPlayer(float gridX, float gridY, Direction facing) override;
    void present() override;
    bool isOpen() const override;

    std::optional<sf::Event> pollEvent() override;

private:
    static constexpr int TILE_SIZE = 32;
    static constexpr int WINDOW_WIDTH = 1024;
    static constexpr int WINDOW_HEIGHT = 768;

    std::unique_ptr<sf::RenderWindow> window_;

    // Color map for terrain types
    struct TerrainColor {
        unsigned char r, g, b;
    };
    std::map<Terrain::Type, TerrainColor> terrainColors_;

    // Helper methods
    void initTerrainColors();
    int screenX(int gridX) const { return gridX * TILE_SIZE; }
    int screenY(int gridY) const { return gridY * TILE_SIZE; }
    float screenX(float gridX) const { return gridX * TILE_SIZE; }
    float screenY(float gridY) const { return gridY * TILE_SIZE; }

    // Direction indicator (arrow pointing in facing direction)
    void drawDirectionIndicator(int screenX, int screenY, Direction facing);
};