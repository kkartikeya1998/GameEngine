// CreatureAISystem.h
#pragma once

#include <cstdlib>
#include <cmath>
#include "entities/Entity.h"
#include "world/World.h"
#include "tmp/component/PositionComponent.h"
#include "tmp/component/VelocityComponent.h"
#include "tmp/component/DirectionComponent.h"
#include "tmp/component/MovementStateComponent.h"
#include "entities/npc/WanderAIComponent.h"
#include "entities/npc/TileRestrictionComponent.h"
#include "system/GameConstants.h"


namespace CreatureAISystem {

inline int toTile(float worldCoord) {
    return static_cast<int>(worldCoord) / GameConstants::TILE_SIZE;
}

inline void pickNewTarget(Entity& e, WanderAIComponent& ai, const PositionComponent& pos) {
    // TODO: wander radius is a placeholder — move to GameConstants or
    // a per-species value once a second creature needs a different one.
    constexpr float WANDER_RADIUS_TILES = 2.f;

    int baseTileX = toTile(pos.x);
    int baseTileY = toTile(pos.y);

    for (int attempt = 0; attempt < 8; ++attempt) {
        int dx = (std::rand() % static_cast<int>(WANDER_RADIUS_TILES * 2 + 1))
                  - static_cast<int>(WANDER_RADIUS_TILES);
        int dy = (std::rand() % static_cast<int>(WANDER_RADIUS_TILES * 2 + 1))
                  - static_cast<int>(WANDER_RADIUS_TILES);
        int candTileX = baseTileX + dx;
        int candTileY = baseTileY + dy;

        if (const auto* restriction = e.get<TileRestrictionComponent>()) {
            if (!restriction->isAllowed(candTileX, candTileY))
                continue; // re-roll — outside allowed tiles
        }

        ai.targetX = (candTileX + 0.5f) * GameConstants::TILE_SIZE;
        ai.targetY = (candTileY + 0.5f) * GameConstants::TILE_SIZE;
        return;
    }
    // No valid tile found after 8 tries — stay put, retry next Idle cycle.
    ai.targetX = pos.x;
    ai.targetY = pos.y;
}

inline void updateOne(Entity& e, float dt) {
    if (!e.has<PositionComponent>() || !e.has<VelocityComponent>()
        || !e.has<DirectionComponent>() || !e.has<MovementStateComponent>())
        return;

    auto& ai = *e.get<WanderAIComponent>();
    auto& pos = *e.get<PositionComponent>();
    auto& vel = *e.get<VelocityComponent>();
    auto& dir = *e.get<DirectionComponent>();
    auto& moveState = *e.get<MovementStateComponent>();

    // TODO: replace with per-species speed once a second species needs
    // a different one; GameConstants::WILD_POKEMON_SPEED doesn't exist
    // yet — placeholder name only.
    constexpr float MOVE_SPEED = GameConstants::WILD_POKEMON_SPEED;

    switch (ai.state) {
        case CreatureState::Idle: {
            vel.vx = 0.f; vel.vy = 0.f;
            moveState.current = MovementState::Idle;
            ai.stateTimer -= dt;
            if (ai.stateTimer <= 0.f) {
                // TODO: jump-vs-move chance is a placeholder ratio.
                if (std::rand() % 5 == 0) {
                    ai.state = CreatureState::Jumping;
                    ai.stateTimer = 0.6f; // TODO: should match jump animation duration
                } else {
                    pickNewTarget(e, ai, pos);
                    ai.state = CreatureState::Moving;
                }
            }
            break;
        }
        case CreatureState::Moving: {
            float toX = ai.targetX - pos.x;
            float toY = ai.targetY - pos.y;
            float dist = std::sqrt(toX * toX + toY * toY);

            if (dist < 2.f) {
                vel.vx = 0.f; vel.vy = 0.f;
                ai.state = CreatureState::Idle;
                ai.stateTimer = 1.f + static_cast<float>(std::rand() % 200) / 100.f; // 1–3s
                moveState.current = MovementState::Idle;
                break;
            }

            vel.vx = (toX / dist) * MOVE_SPEED;
            vel.vy = (toY / dist) * MOVE_SPEED;
            moveState.current = MovementState::Walking;

            // Cardinal-only facing, matching Direction's 4-way enum.
            if (std::abs(toX) > std::abs(toY))
                dir.facing = toX > 0 ? Direction::RIGHT : Direction::LEFT;
            else
                dir.facing = toY > 0 ? Direction::DOWN : Direction::UP;
            break;
        }
        case CreatureState::Jumping: {
            vel.vx = 0.f; vel.vy = 0.f;
            moveState.current = MovementState::Jumping;
            ai.stateTimer -= dt;
            if (ai.stateTimer <= 0.f) {
                ai.state = CreatureState::Idle;
                ai.stateTimer = 1.f;
            }
            break;
        }
    }
}

inline void update(World& world, float dt) {
    for (Entity* e : world.view<WanderAIComponent>())
        updateOne(*e, dt);
}

} // namespace CreatureAISystem