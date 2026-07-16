#pragma once
#include "entities/Registry.h"
#include "events/EventQueue.h"
#include "events/Events.h"
#include "component/PositionComponent.h"
#include "component/DirectionComponent.h"
#include "component/InteractableComponent.h"
#include "component/PlayerControlComponent.h"
#include "component/CollisionComponent.h"
#include "system/GameConstants.h"
#include "log/Logger.h"

namespace InteractionSystem
{
    // Free-movement world: no tile grid, so "interactable" means the
    // player's collision box (inflated by a small margin) overlaps the
    // candidate's collision box — not a facing-direction point check.
    inline void Update(Registry &registry, EntityID player, EventQueue &events, const PlayerControlComponent &control)
    {
        if (!control.interactRequested)
            return;

        LOG_INFO("InteractionSystem: E pressed, checking for target");

        auto *pos = registry.get<PositionComponent>(player);
        auto *playerCollision = registry.get<CollisionComponent>(player);
        if (!pos || !playerCollision)
        {
            LOG_INFO("InteractionSystem: missing pos or dir component on player");
            return;
        }

        // LOG_INFO("InteractionSystem: player at (" + std::to_string(pos->x) + "," + std::to_string(pos->y) +
        //          ") facing " + std::to_string(static_cast<int>(dir->facing)));

        constexpr float kInteractMargin = 8.f; // px of "reach" beyond actual box contact

        AABB playerBox = playerCollision->resolve(pos->x, pos->y);
        AABB reachBox{
            playerBox.x - kInteractMargin,
            playerBox.y - kInteractMargin,
            playerBox.width + kInteractMargin * 2.f,
            playerBox.height + kInteractMargin * 2.f};

        for (EntityID candidate : registry.view<InteractableComponent, PositionComponent>())
        {
            if (candidate.index == player.index && candidate.generation == player.generation)
                continue; // safety guard, shouldn't normally trigger

            auto *cPos = registry.get<PositionComponent>(candidate);
            auto *cCollision = registry.get<CollisionComponent>(candidate);
            if (!cPos || !cCollision)
                continue; // an Interactable with no collision box can't be reached this way

            AABB candidateBox = cCollision->resolve(cPos->x, cPos->y);
            if (reachBox.intersects(candidateBox))
            {
                auto *interactable = registry.get<InteractableComponent>(candidate);
                events.Push(InteractionRequested{player, candidate, interactable->interactionId});
                LOG_INFO("InteractionSystem: HIT -> " + interactable->interactionId);
                return; // first overlap wins
            }
        }
    }
}

// namespace InteractionSystem
// {
//     inline void Update(Registry &registry, EntityID player, EventQueue &events, const PlayerControlComponent &control)
//     {
//         if (!control.interactRequested)
//             return;

//         auto *pos = registry.get<PositionComponent>(player);
//         auto *dir = registry.get<DirectionComponent>(player);
//         if (!pos || !dir)
//         {
//             return;
//         }

//         float targetX = pos->x, targetY = pos->y;
//         switch (dir->facing)
//         {
//         case Direction::UP:
//             targetY -= GameConstants::TILE_SIZE;
//             break;
//         case Direction::DOWN:
//             targetY += GameConstants::TILE_SIZE;
//             break;
//         case Direction::LEFT:
//             targetX -= GameConstants::TILE_SIZE;
//             break;
//         case Direction::RIGHT:
//             targetX += GameConstants::TILE_SIZE;
//             break;
//         case Direction::NONE:
//             break;
//         }

//         constexpr float kHitRadius = GameConstants::TILE_SIZE * 0.5f;
//         int candidateCount = 0;
//         for (EntityID candidate : registry.view<InteractableComponent, PositionComponent>())
//         {
//             ++candidateCount;
//             auto *cPos = registry.get<PositionComponent>(candidate);
//             float dx = cPos->x - targetX, dy = cPos->y - targetY;
//             float distSq = dx * dx + dy * dy;
//             LOG_INFO("InteractionSystem: candidate " + std::to_string(candidate.index) +
//                      " at (" + std::to_string(cPos->x) + "," + std::to_string(cPos->y) +
//                      ") distSq=" + std::to_string(distSq) + " threshold=" + std::to_string(kHitRadius * kHitRadius));
//             if (distSq <= kHitRadius * kHitRadius)
//             {
//                 auto *interactable = registry.get<InteractableComponent>(candidate);
//                 events.Push(InteractionRequested{player, candidate, interactable->interactionId});
//                 LOG_INFO("InteractionSystem: HIT -> " + interactable->interactionId);
//                 return;
//             }
//         }
//     }
// }