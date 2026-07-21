#pragma once

#include <optional>
#include <string>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include "render/ResolvedSprite.h"
#include "render/Camera.h"

enum class RenderAnchor
{
    TopLeft,
    CenterBottom
};

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void clear() = 0;
    virtual void beginWorldView(const Camera &camera) = 0;
    virtual void setDefaultView() = 0;

    // Single draw path for tiles, players, and map objects alike.
    virtual void drawEntity(const ResolvedSprite &sprite, RenderAnchor anchor) = 0;

    virtual void drawDebugRect(float x, float y, float width, float height) = 0;
    virtual void drawRect(float x, float y, float width, float height, sf::Color color, bool screenSpace) = 0;
    virtual void drawText(const sf::Text &text) = 0;

    virtual void present() = 0;
    virtual bool isOpen() const = 0;
    virtual std::optional<sf::Event> pollEvent() = 0;
};