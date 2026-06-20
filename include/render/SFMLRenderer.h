#pragma once

#include <memory>
#include <map>
#include <string>

#include "render/IRenderer.h"
#include "render/TileAtlas.h"
#include "render/SpriteAtlas.h"
#include "render/MapObjectAtlas.h"
#include "world/TileRepository.h"
#include "world/MapObjectRepository.h"

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
}

// ---------------------------------------------------------------------------
// SFMLRenderer — SFML-based concrete implementation of IRenderer.
//
// Owns every repository and atlas it needs by value. This keeps Game's
// constructor free of texture/JSON wiring — Game only passes in file paths.
//
// Construction order matters: repositories must be fully constructed before
// the atlases that reference them, since TileAtlas/MapObjectAtlas hold
// const references into tileRepo_/objectRepo_ respectively. Declaration
// order below is also construction order — do not reorder these members.
// ---------------------------------------------------------------------------
class SFMLRenderer : public IRenderer {
public:
    SFMLRenderer(int windowWidth, int windowHeight,
                 const std::string& tileMetadataPath,
                 const std::string& tileSpritesheetPath,
                 const std::string& objectMetadataPath,
                 const std::string& playerSpritesheetPath);
    ~SFMLRenderer() override;

    void clear() override;
    void drawTile(int gridX, int gridY, Terrain::Type terrain) override;
    void drawPlayer(float gridX, float gridY, Direction facing, float animProgress) override;
    void present() override;
    bool isOpen() const override;

    std::optional<sf::Event> pollEvent() override;

private:
    static constexpr int TILE_SIZE = 64;
    static constexpr int WINDOW_WIDTH = 1024;
    static constexpr int WINDOW_HEIGHT = 768;

    // Repositories first — atlases below hold references into these.
    TileRepository tileRepo_;
    MapObjectRepository objectRepo_;

    // Atlases second — each one's constructor consumes a repository above.
    TileAtlas tileAtlas_;
    SpriteAtlas spriteAtlas_;
    MapObjectAtlas mapObjAtlas_;

    // SFML resources last.
    std::unique_ptr<sf::RenderWindow> window_;

    struct TerrainColor {
        unsigned char r, g, b;
    };
    std::map<Terrain::Type, TerrainColor> terrainColors_;

    void initTerrainColors();
    int screenX(int gridX) const { return gridX * TILE_SIZE; }
    int screenY(int gridY) const { return gridY * TILE_SIZE; }
    float screenX(float gridX) const { return gridX * TILE_SIZE; }
    float screenY(float gridY) const { return gridY * TILE_SIZE; }

    void drawDirectionIndicator(float screenX, float screenY, Direction facing);
};