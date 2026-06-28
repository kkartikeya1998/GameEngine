#include "system/MovementSystem.h"
#include "system/GameConstants.h"

namespace MovementSystem {

// ---- Grid movement ---------------------------------------------------

namespace {
    // Same role as GridMovementMechanics::nextPos — ported verbatim,
    // just taking the component's fields instead of position_.
    Position nextGridPos(const GridMovementComponent& movement, Direction dir)
    {
        Position next{movement.gridX, movement.gridY, movement.facing};
        switch (dir)
        {
        case Direction::UP:    next.y -= 1; break;
        case Direction::DOWN:  next.y += 1; break;
        case Direction::LEFT:  next.x -= 1; break;
        case Direction::RIGHT: next.x += 1; break;
        default: break;
        }
        return next;
    }

    // Same role as GridMovementMechanics::tileBoxAt — ported verbatim.
    AABB tileBoxAt(int gridX, int gridY)
    {
        return AABB{
            static_cast<float>(gridX) * GameConstants::TILE_SIZE,
            static_cast<float>(gridY) * GameConstants::TILE_SIZE,
            static_cast<float>(GameConstants::TILE_SIZE),
            static_cast<float>(GameConstants::TILE_SIZE)
        };
    }
}

void updateGrid(GridMovementComponent& movement, Direction inputDir,
                const std::function<bool(const AABB&)>& isBlocked)
{
    // dt is intentionally not a parameter here — a grid hop is
    // logically instantaneous (the visual lerp over time is
    // RenderSystem's job via GridRenderComponent, not this system's),
    // exactly as GridMovementMechanics::update documented.

    movement.isMoving = false;

    // Edge-trigger gate: only attempt a hop when inputDir is a
    // genuinely NEW non-NONE direction versus what was last acted on.
    // Releasing the key (NONE) clears the gate so the same direction
    // can re-trigger later. Ported verbatim from
    // GridMovementMechanics::update.
    if (inputDir == Direction::NONE) {
        movement.lastInputDir = Direction::NONE;
        return;
    }

    if (inputDir == movement.lastInputDir) {
        // Same direction still held from the frame that already
        // triggered a hop — do nothing until it's released.
        return;
    }

    movement.lastInputDir = inputDir;

    Position next = nextGridPos(movement, inputDir);
    AABB destBox = tileBoxAt(next.x, next.y);

    if (isBlocked(destBox)) {
        // Refused. Facing is NOT updated — matches the original
        // contract exactly: position and facing only ever change
        // together, never independently.
        return;
    }

    movement.facing = inputDir;
    movement.gridX = next.x;
    movement.gridY = next.y;
    movement.isMoving = true;
}

AABB getGridHitbox(const GridMovementComponent& movement)
{
    return tileBoxAt(movement.gridX, movement.gridY);
}

// ---- Free movement ----------------------------------------------------

AABB freeHitboxAt(const FreeMovementComponent& movement, float x, float y)
{
    float boxBottomCenterX = x + movement.hitboxOffsetX;
    float boxBottomCenterY = y + movement.hitboxOffsetY;

    return AABB{
        boxBottomCenterX - movement.hitboxWidth / 2.f,
        boxBottomCenterY - movement.hitboxHeight,
        movement.hitboxWidth,
        movement.hitboxHeight
    };
}

AABB getFreeHitbox(const FreeMovementComponent& movement)
{
    return freeHitboxAt(movement, movement.x, movement.y);
}

void updateFree(FreeMovementComponent& movement, float dt, Direction inputDir,
                const std::function<bool(const AABB&)>& isBlocked)
{
    float dx = 0.f, dy = 0.f;

    switch (inputDir) {
        case Direction::UP:    dy = -movement.speed * dt; break;
        case Direction::DOWN:  dy =  movement.speed * dt; break;
        case Direction::LEFT:  dx = -movement.speed * dt; break;
        case Direction::RIGHT: dx =  movement.speed * dt; break;
        case Direction::NONE:  break;
    }

    // Ported verbatim: facing updates on any nonzero input BEFORE
    // either axis's collision test runs — same as
    // FreeMovementMechanics::update, where this line ran unconditionally
    // ahead of the dx/dy blocked-checks below, independent of whether
    // either axis's move is actually accepted.
    if (inputDir != Direction::NONE) {
        movement.facing = inputDir;
    }

    bool moved = false;

    if (dx != 0.f) {
        AABB testX = freeHitboxAt(movement, movement.x + dx, movement.y);
        if (!isBlocked(testX)) {
            movement.x += dx;
            moved = true;
        }
    }

    if (dy != 0.f) {
        AABB testY = freeHitboxAt(movement, movement.x, movement.y + dy);
        if (!isBlocked(testY)) {
            movement.y += dy;
            moved = true;
        }
    }

    movement.isMoving = moved;
}

} // namespace MovementSystem