#pragma once

// ---------------------------------------------------------------------------
// GameConstants — values needed by more than one subsystem that
// shouldn't live privately inside any single one of them.
//
// TILE_SIZE was previously a private static constexpr inside
// SFMLRenderer — fine while only the renderer needed it. GameController
// now also needs it (to build world-space collision queries for free
// movement via Map::isAreaBlocked), and GameController must stay
// renderer-agnostic (it doesn't include SFMLRenderer.h, by design — see
// IRenderer). Rather than have GameController reach into a renderer
// internal, or duplicate the constant in two places, both now read from
// here.
// ---------------------------------------------------------------------------
namespace GameConstants {
    
    // TILE_SIZE: Used by GameController to set pixels per tile
    // and by SFML renderer to compute sprite scaling
    inline constexpr float TILE_SIZE = 128.f;

    // resolution of the game
    inline constexpr int GAME_RESOLUTION_W = TILE_SIZE*12;
    inline constexpr int GAME_RESOLUTION_H = TILE_SIZE*8;

    // Units pixels and seconds    
    inline constexpr float PLAYER_HITBOX_WIDTH = 25.f;
    inline constexpr float PLAYER_HITBOX_HEIGHT = 15.f;
    inline constexpr float PLAYER_HITBOX_OFFSET_X = 0.f;
    inline constexpr float PLAYER_HITBOX_OFFSET_Y = -25.f;
    inline constexpr float PLAYER_SPEED = 256.f;
}