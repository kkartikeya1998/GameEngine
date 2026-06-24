#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>

// ---------------------------------------------------------------------------
// SpriteRegion — a sub-rectangle within a texture, plus its size for
// scaling. Returned by every concrete atlas's sprite-lookup method.
// ---------------------------------------------------------------------------
struct SpriteRegion {
    sf::Rect<int> subrect;
    sf::Vector2f tile_size;
    const sf::Texture* texture = nullptr;
};

// ---------------------------------------------------------------------------
// Atlas — owns a texture and provides two ways to get a SpriteRegion out
// of it.
//
// (1) loadRegion/findRegion — a name -> SpriteRegion table built directly
//     in code (still used where there's no repository, e.g. SpriteAtlas
//     used to use this before it became JSON-driven; kept for any future
//     atlas that wants hardcoded regions without a repository).
//
// (2) regionFromRepository<Repository>() — the NEW shared method. Every
//     JSON-backed atlas (TileAtlas, SpriteAtlas, MapObjectAtlas) was
//     repeating the same four steps: look up a key in its repository,
//     null-check, throw a consistently-shaped error, then build a
//     SpriteRegion from meta->textureRect. That repetition is now here,
//     once. Each derived atlas only supplies the part that's genuinely
//     different per atlas: HOW to build the lookup key from its domain
//     type (Terrain::Type, Direction+WalkFrame, or a type name string).
//
// Atlas does not own or know about any concrete Repository type. The
// template parameter is deduced from the call site — Atlas stays
// decoupled from TileRepository/SpriteRepository/MapObjectRepository.
// The only requirement on Repository is: `find(key) -> const Metadata*`
// where Metadata has a `textureRect` member.
// ---------------------------------------------------------------------------
class Atlas {
public:
    explicit Atlas(const std::string& spritesheet_path);
    virtual ~Atlas() = default;

    const sf::Texture& texture() const { return *texture_; }

protected:
    void loadRegion(const std::string& name, int x, int y, int w, int h);
    const SpriteRegion& findRegion(const std::string& name) const;

    // Shared lookup-and-build logic for every repository-backed atlas.
    // Repository must expose: const Metadata* find(const KeyT&) const
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
        region.subrect = meta->textureRect;
        region.tile_size = sf::Vector2f(
            static_cast<float>(meta->textureRect.size.x),
            static_cast<float>(meta->textureRect.size.y)
        );
        return region;
    }

private:
    std::unique_ptr<sf::Texture> texture_;
    std::unordered_map<std::string, SpriteRegion> regions_;
};