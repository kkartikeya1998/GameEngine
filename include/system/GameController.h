#pragma once

#include <memory>

#include "world/World.h"
#include "world/MapObjectRepository.h"
#include "entities/player/Player.h"

// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
//
// Responsibilities:
//   - Owns World and Player instances
//   - Places Player on the active map of World
//   - Provides read-only access
//
// ---------------------------------------------------------------------------
class GameController {
public:
    // Initialize with a starting map ID, player position, and a reference
    // to the single shared MapObjectRepository (owned by Game).
    GameController(int startMapId, int playerX, int playerY,
                    MapObjectRepository& objectRepository);

    // Read-only accessors for the game state
    World*  getWorld()       { return &world_;  }
    Entity* getPlayer()      { return &player_; }
    Map*    getActiveMap()   { return world_.getActiveMap(); }

    // Movement on the map or state updates
    void movePlayer(Direction dir);

    // Switch to a new map (called when Player steps on a TeleportPoint).
    // Repositions player on the new map.
    void changeMap(int mapId, int newX, int newY);

    void update(float dt) {
        player_.update(dt);
    }

private:
    World  world_;
    Entity player_;
};