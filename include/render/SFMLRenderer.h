#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <SFML/Graphics/Color.hpp>

#include "system/GameConstants.h"
#include "render/IRenderer.h"
#include "component/RenderComponent.h"

namespace sf
{
    class RenderWindow;
    class Texture;
    class Sprite;
}

// ---------------------------------------------------------------------------
// SFMLRenderer — SFML-based concrete implementation of IRenderer.
//
// Knows nothing about repositories or atlases; every RenderComponent it
// receives already carries a resolved texturePath/textureRect. Its only
// job is turning that into an sf::Sprite and drawing it.
// ---------------------------------------------------------------------------
class SFMLRenderer : public IRenderer
{
public:
    SFMLRenderer(int windowWidth, int windowHeight);
    ~SFMLRenderer() override;

    void clear() override;
    void beginWorldView(const Camera &camera) override;
    void setDefaultView() override;
    void drawEntity(const RenderComponent &render, RenderAnchor anchor) override;
    void drawDebugRect(float x, float y, float width, float height) override;
    void drawRect(float x, float y, float width, float height,
                  sf::Color color, bool screenSpace) override;
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
};