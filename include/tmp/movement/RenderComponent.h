#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "entities/Component.h"
#include "system/GameConstants.h"

struct RenderComponent : public Component {
    std::string name;
    std::string texturePath;
    sf::IntRect textureRect;
    
    int sourceTileSize = GameConstants::TILE_SIZE;

    float renderX = 0.f;
    float renderY = 0.f;

    RenderComponent() = default;

    RenderComponent(
        std::string name,
        std::string texturePath,
        sf::IntRect textureRect,
        int sourceTileSize,
        float renderX,
        float renderY)
        : name(std::move(name))
        , texturePath(std::move(texturePath))
        , textureRect(textureRect)
        , sourceTileSize(sourceTileSize)
        , renderX(renderX)
        , renderY(renderY)
    {}
};