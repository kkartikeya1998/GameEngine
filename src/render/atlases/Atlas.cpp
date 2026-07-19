#include "render/atlases/Atlas.h"
#include "exceptions/EngineExceptions.h"
#include "log/Logger.h"
#include <format>

Atlas::Atlas(const std::filesystem::path& spritesheet_path)
{
    texture_ = std::make_unique<sf::Texture>();
    if (!texture_->loadFromFile(spritesheet_path)) {
        // Mandatory startup resource — every tile/entity drawn through this
        // atlas depends on this spritesheet existing, same category as
        // AssetDatabase's metadata loads. Thrown with full context, not
        // logged here too, so main.cpp's single top-level boundary logs it
        // exactly once.
        throw AssetLoadException("Atlas: failed to load spritesheet: " + spritesheet_path.string());
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
        LOG_ERROR(std::format("Atlas: no region defined for '{}'", name));
        throw std::runtime_error("No region defined for: " + name);
    }
    return it->second;
}