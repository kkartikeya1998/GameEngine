#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <filesystem>

#include "system/GameConstants.h"
#include "exceptions/Result.h"
#include "asset/AssetError.h"

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

    // NOTE: unlike regionFromRepository below, findRegion still throws on a
    // miss. Nothing in the files I've seen (TileAtlas uses
    // regionFromRepository instead) calls findRegion/loadRegion — they may
    // be dead code left over from before the ComponentAssetRepository
    // refactor. Left as-is rather than guessing a Result-based signature
    // that could silently change behavior for a caller I can't see — flag
    // whether this is still live and I'll bring it in line.
    const SpriteRegion& findRegion(const std::string& name) const;

    // Shared lookup-and-build logic for single-texture, repository-backed
    // atlases. Repository must expose: const Metadata* find(const KeyT&) const
    // Metadata must expose: sf::IntRect textureRect
    //
    // Returns Result rather than throwing: this is reachable from
    // TileAtlas::getTileSprite, which RenderSystem::submitTile calls once
    // per visible tile, every frame. A single unresolvable name here is
    // recoverable content data, not grounds to take the whole frame (and
    // via the old uncaught-throw path, the whole session) down.
    template <typename Repository, typename KeyT>
    Result<SpriteRegion, AssetError> regionFromRepository(const Repository& repo,
                                       const KeyT& key,
                                       const std::string& notFoundLabel) const
    {
        const auto* meta = repo.find(key);
        if (!meta) {
            return Result<SpriteRegion, AssetError>::Err(
                AssetError{AssetErrorCode::UnknownId, notFoundLabel, "no render metadata found for this key"});
        }

        SpriteRegion region;
        region.subrect = meta->RenderData.textureRect;
        region.tile_size = sf::Vector2f(
            static_cast<float>(meta->RenderData.textureRect.size.x),
            static_cast<float>(meta->RenderData.textureRect.size.y)
        );
        region.sourceTileSize = meta->RenderData.sourceTileSize;

        return Result<SpriteRegion, AssetError>::Ok(std::move(region));
    }

private:
    std::unique_ptr<sf::Texture> texture_;
    std::unordered_map<std::string, SpriteRegion> regions_;
};