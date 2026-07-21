#pragma once
#include "entities/Registry.h"
#include "component/PositionComponent.h"
#include "component/VelocityComponent.h"
#include "component/MovementStateComponent.h"

// Unconditional dx/dy integration for entities that just drift — no
// collision, no map bounds, no direction/movement-state bookkeeping.
// MovementSystem is for player/AI-driven entities called explicitly per
// EntityID; this is the generic registry-sweep counterpart for anything
// else with a Position+Velocity pair (floating text, particles, simple
// projectiles that don't need collision response).
namespace VelocityIntegrationSystem {

inline void update(Registry &registry, float dt)
{
    for (EntityID id : registry.view<VelocityComponent, PositionComponent>())
    {
        if (registry.get<MovementStateComponent>(id))
            continue;

        auto &vel = *registry.get<VelocityComponent>(id);
        auto &pos = *registry.get<PositionComponent>(id);
        pos.x += vel.vx * dt;
        pos.y += vel.vy * dt;
    }
}

}
