#include "render/MapObjectAtlas.h"
#include <stdexcept>

MapObjectAtlas::MapObjectAtlas(const MapObjectRepository& objectRepository)
    : objectRepository_(objectRepository)
{}

const sf::Texture& MapObjectAtlas::getObjectTexture(const std::string& typeName)
{
    auto cached = textures_.find(typeName);
    if (cached != textures_.end()) {
        return *cached->second;
    }

    const ObjectTypeMetadata* meta = objectRepository_.find(typeName);
    if (!meta) {
        throw std::runtime_error("MapObjectRepository has no metadata for type: " + typeName);
    }

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(meta->texturePath)) {
        throw std::runtime_error("Failed to load object texture: " + meta->texturePath);
    }

    auto [it, inserted] = textures_.emplace(typeName, std::move(texture));
    return *it->second;
}