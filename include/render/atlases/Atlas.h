#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <filesystem>

#include "system/GameConstants.h"

// ---------------------------------------------------------------------------
// SpriteRegion — a sub-rectangle within a texture, plus its size for
// scaling. Returned by every concrete atlas's sprite-lookup method.

// ---------------------------------------------------------------------------
struct SpriteRegion {
    sf::Rect<int> subrect;
    sf::Vector2f tile_size;
    const sf::Texture* texture = nullptr;
    int sourceTileSize = GameConstants::TILE_SIZE;
};

// ---------------------------------------------------------------------------
// Atlas — fetches the render metadata from repositories
// ---------------------------------------------------------------------------
class Atlas {
public:
    explicit Atlas(const std::filesystem::path& spritesheet_path);
    virtual ~Atlas() = default;

    const sf::Texture& texture() const { return *texture_; }

protected:
    void loadRegion(const std::string& name, int x, int y, int w, int h);
    const SpriteRegion& findRegion(const std::string& name) const;

    // Shared lookup-and-build logic for single-texture, repository-backed
    // atlases. Repository must expose: const Metadata* find(const KeyT&) const
    // Metadata must expose: sf::IntRect textureRect
    template <typename Repository, typename KeyT>
    SpriteRegion regionFromRepository(const Repository& repo,
                                       const KeyT& key,
                                       const std::string& notFoundLabel) const
    {
        const auto* meta = repo.find(key);
        if (!meta) {
            throw std::runtime_error("No metadata found for: " + notFoundLabel);
        }

        SpriteRegion region;
        region.subrect = meta->RenderData.textureRect;
        region.tile_size = sf::Vector2f(
            static_cast<float>(meta->RenderData.textureRect.size.x),
            static_cast<float>(meta->RenderData.textureRect.size.y)
        );
        region.sourceTileSize = meta->RenderData.sourceTileSize;
        
        return region;
    }

private:
    std::unique_ptr<sf::Texture> texture_;
    std::unordered_map<std::string, SpriteRegion> regions_;
};