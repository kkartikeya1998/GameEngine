#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>
#include "render/RenderLayer.h"

struct TextRenderComponent {
    std::string text;
    sf::Color color = sf::Color::White;
    unsigned int characterSize = 16;
    RenderLayer layer = RenderLayer::ForegroundEffects;
};