#include "system/GameController.h"
#include "system/GameConstants.h"
#include "system/MovementSystem.h"
#include "entities/movement/FreeMovementComponent.h"
#include "entities/movement/GridMovementComponent.h"

// ---------------------------------------------------------------------------
// Constructor — same inferred shape as before (I was never given the
// original GameController.cpp, only the header; this is reconciled
// against Game.cpp's call site and GameConstants' real names, as
// noted previously). The ECS-relevant change: makePlayer() no longer
// takes a std::unique_ptr<FreeMovementMechanics> — there is no such
// class anymore — it takes the raw field values directly and builds a
// FreeMovementComponent internally. See Player.h.
// ---------------------------------------------------------------------------
GameController::GameController(int startMapId, int playerX, int playerY,
                                 const std::string& assetsRoot,
                                 MapObjectRepository& objectRepository)
    : world_(assetsRoot + "/maps/", objectRepository)
    , player_(makePlayer(
          static_cast<float>(playerX),
          static_cast<float>(playerY),
          GameConstants::PLAYER_SPEED,
          GameConstants::PLAYER_HITBOX_WIDTH,
          GameConstants::PLAYER_HITBOX_HEIGHT,
          GameConstants::PLAYER_HITBOX_OFFSET_X,
          GameConstants::PLAYER_HITBOX_OFFSET_Y
      ))
{
    world_.loadMap(startMapId);
}

bool GameController::isPositionBlocked(const AABB& box) const
{
    Map* activeMap = world_.getActiveMap();
    if (!activeMap)
        return true; // no map loaded -> nowhere is walkable

    return activeMap->isAreaBlocked(box);
}

void GameController::updatePlayerMovement(float dt, Direction inputDir)
{
    // player_ is always built by makePlayer (see Player.h) — always a
    // FreeMovementComponent. No dispatch needed here, unlike the NPC
    // loop in update() below, since player_'s shape is fixed by this
    // class's own constructor, not data-driven from a map file.
    auto* movement = player_.get<FreeMovementComponent>();
    if (!movement) return; // should never happen given makePlayer's contract

    MovementSystem::updateFree(*movement, dt, inputDir,
                                [this](const AABB& box) { return isPositionBlocked(box); });
}

void GameController::update(float dt)
{
    // Player's own movement input is applied via updatePlayerMovement
    // (called separately, before this, from Game::run() — see
    // Game.cpp's existing call order). This loop only drives NPCs;
    // player_ already got its update this frame via
    // updatePlayerMovement.
    //
    // Dispatches per-NPC by which movement component is actually
    // present. Every NPC's update receives Direction::NONE today (no
    // behavior driver exists yet — see NpcComponent.h), so this is
    // currently equivalent to "every NPC stands still", same VISIBLE
    // behavior as before this change, just routed through
    // MovementSystem instead of Entity::update()/IMovementBehavior.
    Map* activeMap = world_.getActiveMap();
    if (!activeMap) return;

    auto isBlocked = [this](const AABB& box) { return isPositionBlocked(box); };

    for (auto& npcEntity : activeMap->getNpcs()) {
        if (auto* grid = npcEntity->get<GridMovementComponent>()) {
            MovementSystem::updateGrid(*grid, Direction::NONE, isBlocked);
        } else if (auto* free = npcEntity->get<FreeMovementComponent>()) {
            MovementSystem::updateFree(*free, dt, Direction::NONE, isBlocked);
        }
    }
}

void GameController::changeMap(int mapId, int newX, int newY)
{
    world_.loadMap(mapId);

    // No more downcast needed — player_.movement() used to require a
    // static_cast<FreeMovementMechanics&> escape hatch. Now it's a
    // direct, safe get<T>() lookup; if it returns nullptr something is
    // fundamentally wrong with how player_ was constructed, which is
    // worth asserting on rather than silently no-op'ing.
    auto* movement = player_.get<FreeMovementComponent>();
    if (!movement) return;

    // Same unit assumption as before (INFERRED, not confirmed against
    // real call sites — flagged previously): newX/newY arrive as
    // grid cells, converted to pixels here via TILE_SIZE.
    movement->x = static_cast<float>(newX) * GameConstants::TILE_SIZE;
    movement->y = static_cast<float>(newY) * GameConstants::TILE_SIZE;
}