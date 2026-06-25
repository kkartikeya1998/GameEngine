#pragma once

#include <memory>
#include <map>
#include <string>

#include "render/IRenderer.h"
#include "render/TileAtlas.h"
#include "render/SpriteAtlas.h"
#include "render/MapObjectAtlas.h"
#include "asset/TileRepository.h"
#include "asset/MapObjectRepository.h"
#include "asset/SpriteRepository.h"

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
}

// ---------------------------------------------------------------------------
// SFMLRenderer — SFML-based concrete implementation of IRenderer.
//
// CHANGED: no longer owns repositories by value. TileRepository,
// MapObjectRepository, and SpriteRepository are now single shared instances
// owned by Game (via AssetManager) and passed in here by reference. This
// removes the duplication that existed when SFMLRenderer parsed the same
// JSON files into its own private copies, separate from World's.
//
// TileRepository was renamed from MapRepository — same class, name only.
//
// SFMLRenderer still owns its three atlases by value — atlases are
// rendering-only concerns (texture + region lookup), genuinely private to
// this renderer, unlike the metadata repositories which are shared facts
// used by both gameplay (MapLoader) and rendering (the atlases).
// ---------------------------------------------------------------------------
class SFMLRenderer : public IRenderer {
public:
    SFMLRenderer(int windowWidth, int windowHeight,
                 const TileRepository& tileRepository,
                 const MapObjectRepository& objectRepository,
                 const SpriteRepository& spriteRepository,
                 const std::string& tileSpritesheetPath,
                 const std::string& playerSpritesheetPath,
                 const std::string& objectSpritesheetPath);
    ~SFMLRenderer() override;
 

    void clear() override;
    void drawTile(int gridX, int gridY, const std::string& typeName) override;
    void drawPlayer(float gridX, float gridY, Direction facing, float animProgress) override;
    void drawMapObject(int gridX, int gridY, const std::string& typeName) override;
    void present() override;
    bool isOpen() const override;

    std::optional<sf::Event> pollEvent() override;

private:
    static constexpr int TILE_SIZE = 64;
    static constexpr int WINDOW_WIDTH = 1024;
    static constexpr int WINDOW_HEIGHT = 768;

    // Atlases — genuinely owned here, each references a repository that
    // Game owns and passed in via the constructor above.
    TileAtlas tileAtlas_;
    SpriteAtlas spriteAtlas_;
    MapObjectAtlas mapObjAtlas_;

    // SFML resources.
    std::unique_ptr<sf::RenderWindow> window_;

    struct TerrainColor {
        unsigned char r, g, b;
    };
    std::map<Terrain::Type, TerrainColor> terrainColors_;

    void initTerrainColors();
    // int screenX(int gridX) const { return gridX * TILE_SIZE; }
    // int screenY(int gridY) const { return gridY * TILE_SIZE; }
    float screenX(float gridX) const { return gridX * TILE_SIZE; }
    float screenY(float gridY) const { return gridY * TILE_SIZE; }

    void drawDirectionIndicator(float screenX, float screenY, Direction facing);
};