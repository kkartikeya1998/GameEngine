#pragma once
#include <string>
#include <SFML/Graphics/Rect.hpp>

// What RenderSystem needs to actually draw one sprite, after resolving
// SpriteAssetComponent + SpriteFrameComponent + world position at
// submit() time. Proposed replacement for IRenderer::drawEntity's
// RenderComponent parameter — needs IRenderer.h/SFMLRenderer.cpp review.
struct ResolvedSprite {
    std::string texturePath;
    sf::IntRect textureRect;
    int sourceTileSize;
    float renderScale;
    float x;
    float y;
};