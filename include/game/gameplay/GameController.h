#pragma once

#include <memory>
#include <string>

#include "game/world/World.h"
#include "engine/ecs/Registry.h"
#include "game/ecs/factories/PlayerFactory.h"
#include "game/ecs/player/PlayerControlComponent.h"
#include "engine/events/EventQueue.h"
#include "game/events/GameEvents.h"

// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
// ---------------------------------------------------------------------------

class GameController
{
public:
    GameController(int startMapId, int playerX, int playerY, const AssetDatabase &assets, EventQueue<GameEvent> &events);

    World *getWorld() { return &world_; }
    EntityID getPlayer() { return playerId_; }
    const Map &getActiveMap() { return world_.getActiveMap(); }

    void update(float dt, const PlayerControlComponent &input);
    void changeMap(int mapId, float newX, float newY);

private:
    const AssetDatabase &assets_;
    EventQueue<GameEvent> &events_;
    World world_;
    EntityID playerId_;
    bool isPositionBlockedFor(EntityID id, const AABB &box);
    void checkItemPickups();

};