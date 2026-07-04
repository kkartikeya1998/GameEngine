#pragma once

#include <string>
#include <SFML/Graphics.hpp>

struct RenderBox {
    std::string name;
    
    std::string texturePath;

    sf::IntRect textureRect;

    int sourceTileSize = 32;
};