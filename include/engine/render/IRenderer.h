#pragma once

#include <optional>
#include <string>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include "engine/render/ResolvedSprite.h"
#include "engine/render/CameraView.h"

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
    virtual void beginWorldView(const CameraView &camera) = 0;
    virtual void setDefaultView() = 0;

    virtual void drawEntity(const ResolvedSprite &sprite, RenderAnchor anchor) = 0;

    virtual void drawDebugRect(float x, float y, float width, float height) = 0;
    virtual void drawRect(float x, float y, float width, float height, sf::Color color, bool screenSpace) = 0;
    virtual void drawText(const sf::Text &text) = 0;

    virtual void present() = 0;
    virtual bool isOpen() const = 0;
    virtual std::optional<sf::Event> pollEvent() = 0;
};