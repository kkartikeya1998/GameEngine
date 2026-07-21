#pragma once
#include "entities/Registry.h"
#include "component/LifetimeComponent.h"

// Destroying while iterating registry.view<>()'s result is safe — view()
// materializes a snapshot vector before the loop runs.
namespace LifetimeSystem {

inline void update(Registry &registry, float dt)
{
    for (EntityID id : registry.view<LifetimeComponent>())
    {
        auto &lifetime = *registry.get<LifetimeComponent>(id);
        lifetime.remainingSeconds -= dt;
        if (lifetime.remainingSeconds <= 0.f)
            registry.destroy(id);
    }
}

}