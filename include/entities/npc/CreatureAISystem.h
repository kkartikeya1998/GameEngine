#pragma once

#include <cstdlib>
#include <cmath>
#include "entities/Registry.h"
#include "component/PositionComponent.h"
#include "component/VelocityComponent.h"
#include "component/DirectionComponent.h"
#include "component/MovementStateComponent.h"
#include "entities/npc/WanderAIComponent.h"
#include "entities/npc/TileRestrictionComponent.h"
#include "system/GameConstants.h"

namespace CreatureAISystem
{

    inline int toTile(float worldCoord)
    {
        return static_cast<int>(worldCoord) / GameConstants::TILE_SIZE;
    }

    inline void pickNewTarget(Registry &registry, EntityID id, WanderAIComponent &ai, const PositionComponent &pos)
    {
        constexpr float WANDER_RADIUS_TILES = 2.f;

        int baseTileX = toTile(pos.x);
        int baseTileY = toTile(pos.y);
        const auto *restriction = registry.get<TileRestrictionComponent>(id);

        for (int attempt = 0; attempt < 8; ++attempt)
        {
            int dx = (std::rand() % static_cast<int>(WANDER_RADIUS_TILES * 2 + 1)) - static_cast<int>(WANDER_RADIUS_TILES);
            int dy = (std::rand() % static_cast<int>(WANDER_RADIUS_TILES * 2 + 1)) - static_cast<int>(WANDER_RADIUS_TILES);
            int candTileX = baseTileX + dx;
            int candTileY = baseTileY + dy;

            if (restriction && !restriction->isAllowed(candTileX, candTileY))
                continue; // re-roll — outside allowed tiles

            ai.targetX = (candTileX + 0.5f) * GameConstants::TILE_SIZE;
            ai.targetY = (candTileY + 0.5f) * GameConstants::TILE_SIZE;
            return;
        }
        ai.targetX = pos.x; // no valid tile after 8 tries — stay put, retry next Idle cycle
        ai.targetY = pos.y;
    }

    inline void updateOne(Registry &registry, EntityID id, float dt)
    {
        auto *posPtr = registry.get<PositionComponent>(id);
        auto *velPtr = registry.get<VelocityComponent>(id);
        auto *dirPtr = registry.get<DirectionComponent>(id);
        auto *moveStatePtr = registry.get<MovementStateComponent>(id);
        auto *aiPtr = registry.get<WanderAIComponent>(id);

        if (!posPtr || !velPtr || !dirPtr || !moveStatePtr || !aiPtr)
            return;

        auto &pos = *posPtr;
        auto &vel = *velPtr;
        auto &dir = *dirPtr;
        auto &moveState = *moveStatePtr;
        auto &ai = *aiPtr;

        constexpr float MOVE_SPEED = GameConstants::WILD_POKEMON_SPEED; // see note below — doesn't exist yet

        switch (ai.state)
        {
        case CreatureState::Idle:
        {
            vel.vx = 0.f;
            vel.vy = 0.f;
            moveState.current = MovementState::Idle;
            ai.stateTimer -= dt;
            if (ai.stateTimer <= 0.f)
            {
                if (std::rand() % 5 == 0)
                {
                    ai.state = CreatureState::Jumping;
                    ai.stateTimer = 0.6f;
                }
                else
                {
                    pickNewTarget(registry, id, ai, pos);
                    ai.state = CreatureState::Moving;
                }
            }
            break;
        }
        case CreatureState::Moving:
        {
            float toX = ai.targetX - pos.x;
            float toY = ai.targetY - pos.y;
            float dist = std::sqrt(toX * toX + toY * toY);

            if (dist < 2.f)
            {
                vel.vx = 0.f;
                vel.vy = 0.f;
                ai.state = CreatureState::Idle;
                ai.stateTimer = 1.f + static_cast<float>(std::rand() % 200) / 100.f;
                ai.movingElapsed = 0.f;
                moveState.current = MovementState::Idle;
                break;
            }

            ai.movingElapsed += dt;
            if (ai.movingElapsed > 2.f)
            { // gave up trying — target unreachable
                vel.vx = 0.f;
                vel.vy = 0.f;
                ai.state = CreatureState::Idle;
                ai.stateTimer = 0.5f;
                ai.movingElapsed = 0.f;
                moveState.current = MovementState::Idle;
                break;
            }

            vel.vx = (toX / dist) * MOVE_SPEED;
            vel.vy = (toY / dist) * MOVE_SPEED;
            moveState.current = MovementState::Walking;

            if (std::abs(toX) > std::abs(toY))
                dir.facing = toX > 0 ? Direction::RIGHT : Direction::LEFT;
            else
                dir.facing = toY > 0 ? Direction::DOWN : Direction::UP;
            break;
        }
        case CreatureState::Jumping:
        {
            vel.vx = 0.f;
            vel.vy = 0.f;
            moveState.current = MovementState::Jumping;
            ai.stateTimer -= dt;
            if (ai.stateTimer <= 0.f)
            {
                ai.state = CreatureState::Idle;
                ai.stateTimer = 1.f;
            }
            break;
        }
        }
    }

    inline void update(Registry &registry, float dt)
    {
        for (EntityID id : registry.view<WanderAIComponent>())
            updateOne(registry, id, dt);
    }

} // namespace CreatureAISystem