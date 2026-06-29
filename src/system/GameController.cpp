#include "system/GameController.h"
#include "system/GameConstants.h"
#include "system/MovementSystem.h"
#include "entities/movement/PositionComponent.h"
#include "entities/movement/FreeMovementComponent.h"
#include "entities/movement/GridMovementComponent.h"

GameController::GameController(int startMapId, int playerX, int playerY,
                                 const std::string& assetsRoot,
                                 MapObjectRepository& objectRepository)
    : world_(objectRepository)
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
    // PositionComponent + FreeMovementComponent pair. No dispatch
    // needed here, unlike the NPC loop in update() below, since
    // player_'s shape is fixed by this class's own constructor, not
    // data-driven from a map file.
    auto* position = player_.get<PositionComponent>();
    auto* movement = player_.get<FreeMovementComponent>();
    if (!position || !movement) return; // should never happen given makePlayer's contract

    MovementSystem::updateFree(*position, *movement, dt, inputDir,
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
    // behavior as before this change.
    //
    // CHANGED (PositionComponent pass): both branches now also fetch
    // PositionComponent and pass it alongside the movement component.
    // If an NPC Entity has a movement component but no
    // PositionComponent, that's a construction bug at the entity's
    // build site (MapLoader.cpp) — skipped rather than crashing, same
    // defensive posture as the existing nullptr checks here.
    Map* activeMap = world_.getActiveMap();
    if (!activeMap) return;

    auto isBlocked = [this](const AABB& box) { return isPositionBlocked(box); };

    for (auto& npcEntity : activeMap->getNpcs()) {
        auto* position = npcEntity->get<PositionComponent>();
        if (!position) continue;

        if (auto* grid = npcEntity->get<GridMovementComponent>()) {
            MovementSystem::updateGrid(*position, *grid, Direction::NONE, isBlocked);
        } else if (auto* free = npcEntity->get<FreeMovementComponent>()) {
            MovementSystem::updateFree(*position, *free, dt, Direction::NONE, isBlocked);
        }
    }
}

void GameController::changeMap(int mapId, int newX, int newY)
{
    world_.loadMap(mapId);

    // CHANGED (PositionComponent pass): writes through
    // PositionComponent now — position->x/y replaces the old
    // movement->x/y direct write, since FreeMovementComponent no
    // longer holds position fields at all.
    auto* position = player_.get<PositionComponent>();
    if (!position) return;

    // Same unit assumption as before (INFERRED, not confirmed against
    // real call sites — flagged previously): newX/newY arrive as
    // grid cells, converted to pixels here via TILE_SIZE.
    position->x = static_cast<float>(newX) * GameConstants::TILE_SIZE;
    position->y = static_cast<float>(newY) * GameConstants::TILE_SIZE;
}