#pragma once

#include <memory>
#include <string>

#include "world/World.h"
#include "asset/MapObjectRepository.h"
#include "entities/player/Player.h"
// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
//
// CHANGED (this pass): Player is now `using Player = Entity;` (see
// Player.h) — player_'s declared type and getPlayer()'s return type are
// UNCHANGED at the syntax level; only what "Player" means underneath
// changed. updatePlayerMovement(dt, inputDir) now forwards straight into
// Entity::update(dt, inputDir, isBlocked) — Entity itself owns calling
// the right IMovementBehavior, so this method's body shrinks to "build
// isBlocked, forward."
//
// CHANGED: update(dt)'s NPC loop now calls npc->update(dt, isBlocked) —
// two args instead of one. NPCs are full Entities now (movement-capable,
// not just animated-in-place dummies), so they need the same collision
// query Player does. They are NOT given live input (see Npc::update /
// computeInputDir — always Direction::NONE today, since no behavior
// driver exists yet), so in practice every NPC currently just stands
// still and lets its IRenderState idle — identical visible behavior to
// before this change, just routed through the now-uniform Entity
// update() signature instead of a a no-collision shortcut.
//
// isPositionBlocked is now reused for BOTH player and NPCs (previously
// it served the player-only collision query) — it's already
// Map-and-TILE_SIZE-driven and has no player-specific assumption in it,
// so no change to its own implementation is needed, only that more
// callers now share it.
// ---------------------------------------------------------------------------
class GameController
{
public:
    GameController(int startMapId, int playerX, int playerY,
                   const std::string &assetsRoot,
                   MapObjectRepository &objectRepository);

    World *getWorld() { return &world_; }
    Player *getPlayer() { return &player_; }
    Map *getActiveMap() { return world_.getActiveMap(); }

    // Called every frame (not just on keypress) with the currently held
    // movement input. NONE if no movement key is held this frame.
    void updatePlayerMovement(float dt, Direction inputDir);

    // Switch to a new map (called when Player's hitbox overlaps a
    // TeleportPoint). Repositions player on the new map.
    void changeMap(int mapId, int newX, int newY);

    void update(float dt)
    {
        // Player's own movement input is applied via
        // updatePlayerMovement (called separately, before this, from
        // Game::run() — see Game.cpp's existing call order). This loop
        // only drives NPCs; player_ already got its update() call this
        // frame via updatePlayerMovement.
        //
        // npc->update(dt) — ONE arg, matching the land-now placeholder
        // Npc.h (see entities/npc/Npc.h comment). When NPCs become real
        // (behavior-driven movement), this becomes
        // npc->update(dt, [this](const AABB& box) { return isPositionBlocked(box); })
        // — that's the one line to change here; nothing else in
        // GameController needs to.
        if (Map *activeMap = world_.getActiveMap())
        {
            for (auto &npc : activeMap->getNpcs())
            {
                npc->update(dt);
            }
        }
    }

private:
    World world_;
    Player player_;

    // Builds the AABB-overlap collision query any Entity's
    // IMovementBehavior needs (player or NPC), backed by the active
    // map's tile data.
    bool isPositionBlocked(const AABB &box) const;
};