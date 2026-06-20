#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>

// ---------------------------------------------------------------------------
// SpriteRegion — a named rectangle within a texture, plus its size for
// scaling. Shared by TileAtlas and SpriteAtlas.
// ---------------------------------------------------------------------------
struct SpriteRegion {
    sf::Rect<int> subrect;
    sf::Vector2f tile_size; // optional, for scaling
};

// ---------------------------------------------------------------------------
// Atlas Base — owns a texture and a name -> SpriteRegion lookup table.
//
// Both TileAtlas and SpriteAtlas need exactly this: load a spritesheet once,
// carve out named rectangles from it, hand them back by name. Neither knows
// or cares what the regions MEAN (terrain vs. walk-cycle frame) — that
// semantic lookup lives in the derived class.
// ---------------------------------------------------------------------------
class Atlas {
public:
    explicit Atlas(const std::string& spritesheet_path);
    virtual ~Atlas() = default;

    const sf::Texture& texture() const { return *texture_; }

protected:
    // Carves out a named rectangle and stores it for later lookup by name.
    void loadRegion(const std::string& name, int x, int y, int w, int h);

    // Looks up a previously-loaded region by name. Throws if not found.
    const SpriteRegion& findRegion(const std::string& name) const;

private:
    std::unique_ptr<sf::Texture> texture_;
    std::unordered_map<std::string, SpriteRegion> regions_;
};