#pragma once

// ---------------------------------------------------------------------------
// GameConstants — values needed by more than one subsystem that
// shouldn't live privately inside any single one of them.
//
// ---------------------------------------------------------------------------
namespace GameConstants {
    
    // TILE_SIZE: Used by GameController to set pixels per tile
    // and by SFML renderer to compute sprite scaling
    inline constexpr float TILE_SIZE = 48.f;

    inline constexpr int TILES_VISIBLE_X = 16;
    inline constexpr int TILES_VISIBLE_Y = TILES_VISIBLE_X * 9 / 16; // 16:9 aspect ratio

    // Units pixels and seconds
    inline constexpr float PLAYER_SPEED = TILE_SIZE*4;

    inline constexpr float WILD_POKEMON_SPEED = 60.0f;

    // resolution of the game
    inline constexpr int GAME_RESOLUTION_W = TILE_SIZE*TILES_VISIBLE_X;
    inline constexpr int GAME_RESOLUTION_H = TILE_SIZE*TILES_VISIBLE_Y;
    inline constexpr int FRAME_RATE = 60;
}