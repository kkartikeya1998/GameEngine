#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <filesystem>
#include <SFML/Graphics/Color.hpp>

#include "system/GameConstants.h"
#include "render/IRenderer.h"
#include "render/ResolvedSprite.h"


namespace sf { class RenderWindow; class Texture; class Sprite; }

class SFMLRenderer : public IRenderer
{
public:
    SFMLRenderer(int windowWidth, int windowHeight);
    ~SFMLRenderer() override;

    void clear() override;
    void beginWorldView(const Camera &camera) override;
    void setDefaultView() override;
    void drawEntity(const ResolvedSprite &sprite, RenderAnchor anchor) override;
    void drawDebugRect(float x, float y, float width, float height) override;
    void drawRect(float x, float y, float width, float height, sf::Color color, bool screenSpace) override;
    void drawText(const sf::Text& text) override;
    void present() override;
    bool isOpen() const override;
    std::optional<sf::Event> pollEvent() override;

private:
    static constexpr int TILE_SIZE = GameConstants::TILE_SIZE;
    static constexpr int WINDOW_WIDTH = GameConstants::GAME_RESOLUTION_W;
    static constexpr int WINDOW_HEIGHT = GameConstants::GAME_RESOLUTION_H;

    std::unique_ptr<sf::RenderWindow> window_;

    // Textures cached by path so repeated entities sharing a sheet load it once
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textureCache_;
    const sf::Texture &getOrLoadTexture(const std::string &path);

    // Fallback for a texture path that fails to load. Built lazily, once,
    // on first miss — most sessions never need it.
    std::unique_ptr<sf::Texture> missingTexture_;
    std::unique_ptr<sf::Texture> buildMissingTexture();

    // A broken texture path can be referenced by something drawn every
    // frame (a tile, a common sprite). Without this, that's an ERROR log
    // line at 60fps for as long as the map is loaded. Log once per unique
    // path instead.
    std::unordered_set<std::string> warnedMissingTextures_;
};