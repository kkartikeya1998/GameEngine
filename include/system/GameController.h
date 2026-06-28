#pragma once

#include <memory>
#include <string>

#include "world/World.h"
#include "asset/MapObjectRepository.h"
#include "entities/Entity.h"
#include "entities/player/Player.h"

// ---------------------------------------------------------------------------
// GameController — manages World and Player lifecycle.
//
// CHANGED (ECS pass): Player is no longer a type at all (see Player.h —
// `using Player = Entity;` is gone, replaced by makePlayer() returning
// a plain Entity tagged with PlayerControlComponent). player_'s
// declared type changes from Player to Entity; getPlayer()'s return
// type changes from Player* to Entity*. Every external caller that
// held a `Player*` (RenderSystem.cpp chiefly) now holds `Entity*` and
// reads whatever components it needs via get<T>() — see
// RenderSystem.cpp's ported version.
//
// updatePlayerMovement(dt, inputDir) now looks up player_'s
// FreeMovementComponent directly and calls
// MovementSystem::updateFree() on it — there is no more
// Entity::update() method to forward to (Entity holds no behavior of
// its own anymore), so GameController is now the place that decides
// WHICH movement system function to call. For player_ specifically
// this is always updateFree() (player_ is always built by makePlayer,
// which only ever attaches FreeMovementComponent) — no dispatch
// needed, unlike the NPC loop below.
//
// update(dt)'s NPC loop dispatches per-NPC by which movement component
// is actually present (GridMovementComponent vs FreeMovementComponent),
// since NPCs are no longer guaranteed to be one specific kind the way
// player_ is. Today this is moot (no NPC behavior driver exists yet,
// so every NPC's update call always receives Direction::NONE — same
// "stands still" visible behavior as before this change), but the
// dispatch is real and will matter the moment NPCs gain a behavior
// driver, without GameController needing to change again then.
//
// isPositionBlocked is unchanged — still Map-and-TILE_SIZE-driven, no
// player-specific assumption, shared by player_ and every NPC exactly
// as before.
// ---------------------------------------------------------------------------
class GameController
{
public:
    GameController(int startMapId, int playerX, int playerY,
                   const std::string &assetsRoot,
                   MapObjectRepository &objectRepository);

    World *getWorld() { return &world_; }
    Entity *getPlayer() { return &player_; }
    Map *getActiveMap() { return world_.getActiveMap(); }

    // Called every frame (not just on keypress) with the currently held
    // movement input. NONE if no movement key is held this frame.
    void updatePlayerMovement(float dt, Direction inputDir);

    // Switch to a new map (called when Player's hitbox overlaps a
    // TeleportPoint). Repositions player on the new map.
    void changeMap(int mapId, int newX, int newY);

    void update(float dt);

private:
    World world_;
    Entity player_;

    // Builds the AABB-overlap collision query any movement component
    // needs (player or NPC), backed by the active map's tile data.
    bool isPositionBlocked(const AABB &box) const;
};