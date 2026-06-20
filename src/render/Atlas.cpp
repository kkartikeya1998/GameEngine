#include "render/Atlas.h"
#include <stdexcept>

Atlas::Atlas(const std::string& spritesheet_path)
{
    texture_ = std::make_unique<sf::Texture>();
    if (!texture_->loadFromFile(spritesheet_path)) {
        throw std::runtime_error("Failed to load spritesheet: " + spritesheet_path);
    }
}

void Atlas::loadRegion(const std::string& name, int x, int y, int w, int h)
{
    SpriteRegion region;
    region.subrect = sf::Rect<int>(
        sf::Vector2<int>(x, y),
        sf::Vector2<int>(w, h)
    );
    region.tile_size = sf::Vector2f(w, h);
    regions_[name] = region;
}

const SpriteRegion& Atlas::findRegion(const std::string& name) const
{
    auto it = regions_.find(name);
    if (it == regions_.end()) {
        throw std::runtime_error("No region defined for: " + name);
    }
    return it->second;
}