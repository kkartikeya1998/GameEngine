#pragma once

#include <memory>
#include <map>
#include <string>
#include <filesystem>

#include "system/GameConstants.h"
#include "render/IRenderer.h"
#include "render/atlases/TileAtlas.h"
#include "render/atlases/SpriteAtlas.h"
#include "render/atlases/MapObjectAtlas.h"
#include "asset/repositories/TileRepository.h"
#include "asset/repositories/MapObjectRepository.h"
#include "asset/repositories/CharacterRepository.h"
#include "tmp/movement/RenderComponent.h"

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
}

// ---------------------------------------------------------------------------
// SFMLRenderer — SFML-based concrete implementation of IRenderer.
//
// CHANGED: no longer owns repositories by value. TileRepository,
// MapObjectRepository, and CharacterRepository are now single shared instances
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
                 const CharacterRepository& characterRepository,
                 const std::filesystem::path& tileSpritesheetPath,
                 const std::filesystem::path& playerSpritesheetPath,
                 const std::filesystem::path& objectSpritesheetPath);
    ~SFMLRenderer() override;
 

    void clear() override;
    void drawTile(int gridX, int gridY, const RenderComponent& tileRender) override;
    void drawPlayer(const PositionComponent& playerPos, const DirectionComponent& facing, const RenderComponent& playerRender, float animProgress) override;
    void drawMapObject(const RenderComponent& objectRender) override;
    void drawDebugRect(float x, float y, float width, float height) override;    void present() override;
    bool isOpen() const override;

    std::optional<sf::Event> pollEvent() override;

private:
    static constexpr int TILE_SIZE = GameConstants::TILE_SIZE;
    static constexpr int WINDOW_WIDTH = GameConstants::GAME_RESOLUTION_W;
    static constexpr int WINDOW_HEIGHT = GameConstants::GAME_RESOLUTION_H;

    // Atlases — genuinely owned here, each references a repository that
    // Game owns and passed in via the constructor above.
    TileAtlas tileAtlas_;
    SpriteAtlas spriteAtlas_;
    MapObjectAtlas mapObjAtlas_;

    // SFML resources.
    std::unique_ptr<sf::RenderWindow> window_;

    // for tiles
    float screenX(float gridX) const { return gridX * TILE_SIZE; }
    float screenY(float gridY) const { return gridY * TILE_SIZE; }
    // for players/npcs
    float spriteScale(float width, float height) { return std::min( static_cast<float>(TILE_SIZE) / width, static_cast<float>(TILE_SIZE) / height); }
};