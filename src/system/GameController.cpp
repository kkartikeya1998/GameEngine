#include "system/GameController.h"
#include "system/GameConstants.h"
#include "entities/movement/FreeMovementMechanics.h"

// ---------------------------------------------------------------------------
// NOTE: I was never given the original GameController.cpp — only the
// header. The constructor body below is INFERRED from:
//   - Game.cpp's construction call:
//       controller_(1, 0, 5, assetsRoot_, assets_.get<MapObjectRepository>())
//   - World's constructor: World(mapsFolder, objectRepository)
//   - The header's own comment: "GameController stays renderer-agnostic:
//     it builds its own world-space collision query from
//     Map::isAreaBlocked using GameConstants::TILE_SIZE"
//
// Hitbox/speed values are GameConstants::PLAYER_SPEED /
// PLAYER_HITBOX_WIDTH/HEIGHT/OFFSET_X/OFFSET_Y — confirmed against the
// real GameConstants.h, not guessed. If your actual GameController.cpp
// constructs FreeMovementMechanics with a different argument order or
// additional setup beyond this, reconcile against that — this body is
// otherwise a faithful match.
// ---------------------------------------------------------------------------
GameController::GameController(int startMapId, int playerX, int playerY,
                                 const std::string& assetsRoot,
                                 MapObjectRepository& objectRepository)
    : world_(assetsRoot + "/maps/", objectRepository)
    , player_(makePlayer(
          std::make_unique<FreeMovementMechanics>(
              static_cast<float>(playerX),
              static_cast<float>(playerY),
              GameConstants::PLAYER_SPEED,
              GameConstants::PLAYER_HITBOX_WIDTH,
              GameConstants::PLAYER_HITBOX_HEIGHT,
              GameConstants::PLAYER_HITBOX_OFFSET_X,
              GameConstants::PLAYER_HITBOX_OFFSET_Y
          )
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
    player_.update(dt, inputDir,
                    [this](const AABB& box) { return isPositionBlocked(box); });
}

void GameController::changeMap(int mapId, int newX, int newY)
{
    world_.loadMap(mapId);

    // player_.movement() downcast to the concrete mechanic to
    // reposition directly — same escape-hatch pattern Npc::current()
    // used before. FreeMovementMechanics::setPosition takes pixel
    // coordinates, same unit newX/newY arrive in per the constructor
    // above (grid-cell args * TILE_SIZE) — INFERRED to match the
    // constructor's own unit choice; reconcile if your real call sites
    // pass pixels directly instead of grid cells.
    auto& movement = static_cast<FreeMovementMechanics&>(player_.movement());
    movement.setPosition(
        static_cast<float>(newX) * GameConstants::TILE_SIZE,
        static_cast<float>(newY) * GameConstants::TILE_SIZE
    );
}