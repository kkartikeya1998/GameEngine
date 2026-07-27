#pragma once

// ---------------------------------------------------------------------------
// GameConstants — values needed by more than one subsystem that
// shouldn't live privately inside any single one of them.
//
// ---------------------------------------------------------------------------
namespace GameConstants {
    
    // TILE_SIZE: Used by GameController to set pixels per tile
    // and by SFML renderer to compute sprite scaling
    inline constexpr float TILE_SIZE = 24.f;

    // resolution of the game
    inline constexpr int GAME_RESOLUTION_W = 900;
    inline constexpr int GAME_RESOLUTION_H = 800;
    inline constexpr int FRAME_RATE = 60;
    inline constexpr int TILES_VISIBLE_X = 10; // TILES_VISIBLE_Y is derived from TILES_VISIBLE_X and the window's aspect ratio 

    // Units pixels and seconds
    inline constexpr float PLAYER_SPEED = TILE_SIZE*4;

    inline constexpr float WILD_POKEMON_SPEED = 60.0f;
}