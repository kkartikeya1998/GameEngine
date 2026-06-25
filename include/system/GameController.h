#pragma once

#include <memory>
#include <string>

#include "world/World.h"
#include "asset/MapObjectRepository.h"
#include "entities/player/Player.h"

// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
//
// CHANGED: player_ is now a Player (FreeEntity-based, continuous
// movement) instead of a plain Entity wrapping GridMovementMechanics.
//
// movePlayer(Direction dir) — the old discrete, commit-or-refuse,
// called-once-per-keypress method — is REMOVED. It assumed
// GridEntity's move(dir)/nextPos(dir) contract, which Player no longer
// implements (FreeEntity has no such method — see IFreeMovementMechanics).
// It's replaced by updatePlayerMovement(dt, inputDir), called every
// frame from Game::run() regardless of whether a key was just pressed
// or is being held.
//
// GameController stays renderer-agnostic: it builds its own
// world-space collision query from Map::isAreaBlocked using
// GameConstants::TILE_SIZE, the same shared constant SFMLRenderer reads
// — it does not include or know about SFMLRenderer.
// ---------------------------------------------------------------------------
class GameController {
public:
    GameController(int startMapId, int playerX, int playerY,
                    const std::string& assetsRoot,
                    MapObjectRepository& objectRepository);

    World*  getWorld()       { return &world_;  }
    Player* getPlayer()      { return &player_; }
    Map*    getActiveMap()   { return world_.getActiveMap(); }

    // Called every frame (not just on keypress) with the currently held
    // movement input. NONE if no movement key is held this frame.
    void updatePlayerMovement(float dt, Direction inputDir);

    // Switch to a new map (called when Player's hitbox overlaps a
    // TeleportPoint). Repositions player on the new map.
    void changeMap(int mapId, int newX, int newY);

    void update(float dt) {
        player_.update(dt);
    }

private:
    World  world_;
    Player player_;

    // Builds the AABB-overlap collision query Player's FreeMovementMechanics
    // needs, backed by the active map's tile data.
    bool isPositionBlocked(const AABB& box) const;
};