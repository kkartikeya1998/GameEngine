#include "render/atlases/MapObjectAtlas.h"
#include <stdexcept>

MapObjectAtlas::MapObjectAtlas(const std::filesystem::path& spritesheet_path, const MapObjectRepository& objectRepository)
    : Atlas(spritesheet_path)
    , objectRepository_(objectRepository)
{}

SpriteRegion MapObjectAtlas::getObjectSprite(const std::string& typeName) const
{
    const ObjectTypeMetadata* meta = objectRepository_.find(typeName);
    if (!meta) {
        throw std::runtime_error("No metadata found for: " + typeName);
    }

    SpriteRegion region;
    region.subrect = meta->textureRect;
    region.tile_size = sf::Vector2f(
        static_cast<float>(meta->textureRect.size.x),
        static_cast<float>(meta->textureRect.size.y)
    );
    region.sourceTileSize = meta->sourceTileSize;
    region.texture = &getOrLoadTexture(meta->texturePath);
    return region;
}

const sf::Texture& MapObjectAtlas::getOrLoadTexture(const std::string& path) const
{
    auto it = textureCache_.find(path);
    if (it != textureCache_.end()) {
        return *it->second;
    }

    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromFile(path)) {
        throw std::runtime_error("Failed to load object texture: " + path);
    }

    const sf::Texture& ref = *tex;
    textureCache_[path] = std::move(tex);
    return ref;
}