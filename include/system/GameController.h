#pragma once

#include <memory>
#include <string>

#include "world/World.h"
#include "asset/MapObjectRepository.h"
#include "entities/player/Player.h"

// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
//
// Responsibilities:
//   - Owns World and Player instances
//   - Places Player on the active map of World
//   - Provides read-only access
//
// CHANGED: constructor now takes assetsRoot explicitly instead of building
// its own path from PROJECT_ROOT internally. Game is the single source of
// truth for where assets live on disk (see Game.h) — GameController should
// not independently know or guess that path.
// ---------------------------------------------------------------------------
class GameController {
public:
    // Initialize with a starting map ID, player position, the assets root
    // path (used to locate maps/), and a reference to the single shared
    // MapObjectRepository (owned by Game via AssetManager).
    GameController(int startMapId, int playerX, int playerY,
                    const std::string& assetsRoot,
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