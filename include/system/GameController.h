#pragma once

#include <memory>

#include "world/World.h"
#include "entities/player/Player.h"

// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
//
// Responsibilities:
//   - Owns World and Player instances
//   - Places Player on the active map of World
//   - Provides read-only access 
//CA
// The controller is the central state holder that ties the game together for now.
// ---------------------------------------------------------------------------
class GameController {
public:
    // Initialize with a starting map ID. Loads that map into World
    // and places the Player at the given tile coordinates.
    GameController(int startMapId, int playerX, int playerY);

    // Read-only accessors for the game state
    World*  getWorld()       { return &world_;  }
    Player* getPlayer()      { return &player_; }
    Map*   getActiveMap()   { return world_.getActiveMap(); }

    // Switch to a new map (called when Player steps on a TeleportPoint).
    // Repositions player on the new map.
    void changeMap(int mapId, int newX, int newY);

    

private:
    //  If I want to enable world switching at runtime, or enable save states, or multiplayer 
    //  I might want to make these unique_ptrs and manage their lifecycles more carefully.
    World  world_;
    Player player_;
};