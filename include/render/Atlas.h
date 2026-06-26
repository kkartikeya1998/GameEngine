#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include "system/GameConstants.h"

// ---------------------------------------------------------------------------
// SpriteRegion — a sub-rectangle within a texture, plus its size for
// scaling. Returned by every concrete atlas's sprite-lookup method.
//
// texture: defaults to nullptr, meaning "use the atlas's own single
// texture()" — TileAtlas and SpriteAtlas never set this, so their callers
// (drawTile, drawPlayer) are completely unaffected by this field.
// MapObjectAtlas is the one atlas that needs per-region textures: object
// types can each point at a different image file (or share one), so its
// SpriteRegion sets texture explicitly. See MapObjectAtlas.h for why.
//
// sourceTileSize: pixels-per-tile in THIS region's own source art.
// Defaults to 32 only as a last-resort fallback for any caller that
// doesn't populate it; every real metadata-backed lookup (tiles, objects)
// sets it from the metadata's own sourceTileSize field. This is what
// lets SFMLRenderer compute scale as TILE_SIZE / sourceTileSize instead
// of assuming a fixed pixels-per-tile ratio for the whole project.
// ---------------------------------------------------------------------------
struct SpriteRegion {
    sf::Rect<int> subrect;
    sf::Vector2f tile_size;
    const sf::Texture* texture = nullptr;
    int sourceTileSize = GameConstants::TILE_SIZE;
};

// ---------------------------------------------------------------------------
// has_source_tile_size<T> — detects whether a Metadata type exposes a
// sourceTileSize member, so regionFromRepository can read it when present
// (TileTypeMetadata, ObjectTypeMetadata) and fall back to SpriteRegion's
// own default when absent (SpriteFrameMetadata has no tile-spanning
// concept — a player animation frame is never "N tiles wide").
// ---------------------------------------------------------------------------
template <typename T, typename = void>
struct has_source_tile_size : std::false_type {};

template <typename T>
struct has_source_tile_size<T, std::void_t<decltype(std::declval<T>().sourceTileSize)>>
    : std::true_type {};

// ---------------------------------------------------------------------------
// Atlas — owns a texture and provides two ways to get a SpriteRegion out
// of it.
//
// (1) loadRegion/findRegion — a name -> SpriteRegion table built directly
//     in code (still used where there's no repository, e.g. SpriteAtlas
//     used to use this before it became JSON-driven; kept for any future
//     atlas that wants hardcoded regions without a repository).
//
// (2) regionFromRepository<Repository>() — the shared method for atlases
//     backed by ONE texture. Every JSON-backed atlas with a single shared
//     spritesheet (TileAtlas, SpriteAtlas) was repeating the same four
//     steps: look up a key in its repository, null-check, throw a
//     consistently-shaped error, then build a SpriteRegion from
//     meta->textureRect. That repetition lives here, once.
//
//     MapObjectAtlas does NOT use this helper — it needs to resolve a
//     texture per-lookup (different object types can point at different
//     image files), which this helper's single-texture assumption can't
//     express. See MapObjectAtlas for its own lookup-and-build logic.
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
        region.subrect = meta->textureRect;
        region.tile_size = sf::Vector2f(
            static_cast<float>(meta->textureRect.size.x),
            static_cast<float>(meta->textureRect.size.y)
        );

        // Only metadata types that actually declare sourceTileSize
        // populate it here (TileTypeMetadata, ObjectTypeMetadata).
        // SpriteFrameMetadata has no such field — has_source_tile_size
        // resolves to false_type for it, this branch is skipped at
        // compile time, and region.sourceTileSize keeps its default.
        if constexpr (has_source_tile_size<std::remove_cv_t<
                           std::remove_pointer_t<decltype(meta)>>>::value) {
            region.sourceTileSize = meta->sourceTileSize;
        }

        return region;
    }

private:
    std::unique_ptr<sf::Texture> texture_;
    std::unordered_map<std::string, SpriteRegion> regions_;
};