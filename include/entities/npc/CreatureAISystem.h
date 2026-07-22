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
    constexpr float WANDER_RADIUS_TILES = 2.f;
    constexpr float ARRIVE_THRESHOLD = 2.f;
    constexpr float MOVING_TIMEOUT = 2.f;
    constexpr float JUMP_DURATION = 0.6f;
    constexpr float IDLE_TIMER_MIN = 2.f;
    constexpr float IDLE_TIMER_MAX_ADD = 2.f;
    constexpr float IDLE_AFTER_TIMEOUT = 0.5f;
    constexpr int JUMP_CHANCE_DENOMINATOR = 5;

    inline int toTile(float worldCoord)
    {
        return static_cast<int>(worldCoord) / GameConstants::TILE_SIZE;
    }

    inline void stopAndEnterIdle(VelocityComponent &vel, MovementStateComponent &moveState,
                                  WanderAIComponent &ai, float timer)
    {
        vel.vx = 0.f;
        vel.vy = 0.f;
        moveState.current = MovementState::Idle;
        ai.state = CreatureState::Idle;
        ai.stateTimer = timer;
        ai.movingElapsed = 0.f;
    }

    inline void pickNewTarget(Registry &registry, EntityID id, WanderAIComponent &ai, const PositionComponent &pos)
    {
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
                continue;

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
        auto &moveState = *moveStatePtr;
        auto &ai = *aiPtr;
        (void)dirPtr; // presence-checked above; DirectionSystem writes facing, not this system

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
                if (std::rand() % JUMP_CHANCE_DENOMINATOR == 0)
                {
                    ai.state = CreatureState::Jumping;
                    ai.stateTimer = JUMP_DURATION;
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

            if (dist < ARRIVE_THRESHOLD)
            {
                stopAndEnterIdle(vel, moveState, ai, IDLE_TIMER_MIN + static_cast<float>(std::rand() % 200) / 100.f);
                break;
            }

            ai.movingElapsed += dt;
            if (ai.movingElapsed > MOVING_TIMEOUT)
            { // gave up trying — target unreachable
                stopAndEnterIdle(vel, moveState, ai, IDLE_AFTER_TIMEOUT);
                break;
            }

            vel.vx = (toX / dist) * ai.wanderSpeed;
            vel.vy = (toY / dist) * ai.wanderSpeed;
            moveState.current = MovementState::Walking;

            // Facing is no longer decided here — DirectionSystem::update
            // resolves the full 8-way facing from vel.vx/vel.vy for every
            // entity with a DirectionComponent, this one included. It must
            // run after this system each frame (see GameController::update)
            // so it picks up the velocity just set above.
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
                ai.stateTimer = IDLE_TIMER_MIN;
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