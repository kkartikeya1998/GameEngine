#pragma once

#include <memory>
#include <string>

#include "world/World.h"
// #include "asset/repositories/MapObjectRepository.h"
// #include "asset/repositories/PokemonSpeciesAssetRepository.h"
#include "entities/Entity.h"
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
    Entity *getPlayer() { return &player_; }
    const Map *getActiveMap() { return world_.getActiveMap(); }

    void update(float dt, const PlayerControlComponent &input);
    void changeMap(int mapId, float newX, float newY);

private:
    World world_;
    Entity player_;
    bool isPositionBlocked(const AABB &box) const;
};
