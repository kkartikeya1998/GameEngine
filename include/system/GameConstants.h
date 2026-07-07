#pragma once

// ---------------------------------------------------------------------------
// GameConstants — values needed by more than one subsystem that
// shouldn't live privately inside any single one of them.
//
// ---------------------------------------------------------------------------
namespace GameConstants {
    
    // TILE_SIZE: Used by GameController to set pixels per tile
    // and by SFML renderer to compute sprite scaling
    inline constexpr float TILE_SIZE = 128.f;

    // resolution of the game
    inline constexpr int GAME_RESOLUTION_W = TILE_SIZE*12;
    inline constexpr int GAME_RESOLUTION_H = TILE_SIZE*8;
    inline constexpr int FRAME_RATE = 60;
    inline constexpr int TILES_VISIBLE_X = 5; // TILES_VISIBLE_Y is derived from TILES_VISIBLE_X and the window's aspect ratio 

    // Units pixels and seconds
    // works good only for 128 tile size
    inline constexpr float PLAYER_HITBOX_WIDTH = 25.f;
    inline constexpr float PLAYER_HITBOX_HEIGHT = 15.f;
    inline constexpr float PLAYER_HITBOX_OFFSET_X = 0.f;
    inline constexpr float PLAYER_HITBOX_OFFSET_Y = -25.f;
    inline constexpr float PLAYER_SPEED = 256.f;
}