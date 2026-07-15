#pragma once

#include <memory>
#include <string>

#include "world/World.h"
#include "entities/Registry.h"
#include "entities/player/Player.h"
#include "component/PlayerControlComponent.h"

// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
// ---------------------------------------------------------------------------

class GameController
{
public:
    GameController(int startMapId, int playerX, int playerY, const AssetDatabase &assets);

    World *getWorld() { return &world_; }
    EntityID getPlayer() { return playerId_; }
    const Map &getActiveMap() { return world_.getActiveMap(); }

    void update(float dt, const PlayerControlComponent &input);
    void changeMap(int mapId, float newX, float newY);

private:
    const AssetDatabase &assets_;
    World world_;
    EntityID playerId_;
    bool isPositionBlockedFor(EntityID id, const AABB &box);
};