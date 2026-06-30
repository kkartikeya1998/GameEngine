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
    , player_(makePlayer(static_cast<float>(playerX), static_cast<float>(playerY)))
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
    auto* position  = player_.get<PositionComponent>();
    auto* velocity  = player_.get<VelocityComponent>();
    auto* direction = player_.get<DirectionComponent>();
    auto* movement  = player_.get<FreeMovementComponent>();
    if (!position || !velocity || !direction || !movement) return;

    MovementSystem::updateFree(*position, *velocity, *direction, *movement, dt, inputDir,
                               [this](const AABB& box) { return isPositionBlocked(box); });
}

void GameController::update(float dt)
{
    Map* activeMap = world_.getActiveMap();
    if (!activeMap) return;

    // NPC and other animates update
    // auto isBlocked = [this](const AABB& box) { return isPositionBlocked(box); };

    // for (auto& npcEntity : activeMap->getNpcs()) {
    //     auto* position = npcEntity->get<PositionComponent>();
    //     if (!position) continue;

    //     if (auto* grid = npcEntity->get<GridMovementComponent>()) {
    //         MovementSystem::updateGrid(*position, *grid, Direction::NONE, isBlocked);
    //     } else if (auto* free = npcEntity->get<FreeMovementComponent>()) {
    //         MovementSystem::updateFree(*position, *free, dt, Direction::NONE, isBlocked);
    //     }
    // }
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