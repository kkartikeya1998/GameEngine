#pragma once
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include "system/GameConstants.h"

// The part of a sprite's presentation that changes over time. Written
// every frame by AnimationSystem for animated entities; written once
// at creation for static entities.
struct SpriteFrameComponent {
    std::string name;         // current runtime frame name (debug/lookup)
    std::string texturePath;
    sf::IntRect textureRect;
    int sourceTileSize = GameConstants::TILE_SIZE;

    SpriteFrameComponent() = default;
    SpriteFrameComponent(std::string name, std::string texturePath, sf::IntRect textureRect, int sourceTileSize)
        : name(std::move(name)), texturePath(std::move(texturePath)), textureRect(textureRect), sourceTileSize(sourceTileSize) {}
};