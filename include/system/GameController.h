#pragma once

#include <memory>
#include <string>

#include "world/World.h"
#include "asset/repositories/MapObjectRepository.h"
#include "entities/Entity.h"
#include "entities/player/Player.h"

// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
// ---------------------------------------------------------------------------
class GameController
{
public:
    GameController(int startMapId, int playerX, int playerY,
                   const std::string &assetsRoot,
                   MapObjectRepository &objectRepository,
                   TileRepository &tileRepository);

    World *getWorld() { return &world_; }
    Entity *getPlayer() { return &player_; }
    Map *getActiveMap() { return world_.getActiveMap(); }

    // Called every frame (not just on keypress) with the currently held
    // movement input. NONE if no movement key is held this frame.
    void updatePlayerMovement(float dt, Direction inputDir);

    // Switch to a new map (called when Player's hitbox overlaps a
    // TeleportPoint). Repositions player on the new map.
    void changeMap(int mapId, float newX, float newY);

    void update(float dt);

private:
    World world_;
    Entity player_;

    // Builds the AABB-overlap collision query any movement component
    // needs (player or NPC), backed by the active map's tile data.
    bool isPositionBlocked(const AABB &box) const;
};