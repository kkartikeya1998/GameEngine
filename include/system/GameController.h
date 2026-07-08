#pragma once

#include <memory>
#include <string>

#include "world/World.h"
#include "asset/repositories/MapObjectRepository.h"
#include "entities/Entity.h"
#include "entities/player/Player.h"
#include "tmp/component/PlayerControlComponent.h"

// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
// ---------------------------------------------------------------------------
class GameController
{
public:
    GameController(int startMapId, int playerX, int playerY,
                   MapObjectRepository &objectRepository,
                   TileRepository &tileRepository);

    World *getWorld() { return &world_; }
    Entity *getPlayer() { return &player_; }
    const Map *getActiveMap() { return world_.getActiveMap(); }

    // Called every frame with the current player input snapshot.
    void update(float dt, const PlayerControlComponent &input);

    // Switch to a new map (called when Player's hitbox overlaps a
    // TeleportPoint). Repositions player on the new map.
    void changeMap(int mapId, float newX, float newY);

private:
    World world_;
    Entity player_;

    bool isPositionBlocked(const AABB &box) const;
};