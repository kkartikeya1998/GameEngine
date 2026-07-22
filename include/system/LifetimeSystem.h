#pragma once
#include "entities/Registry.h"
#include "component/LifetimeComponent.h"

// Ticks down any entity carrying a LifetimeComponent and destroys it at zero.
// Generic on purpose (second-pressure already met: floating heal-amount
// popups today, dropped-item despawn / damage numbers are the obvious next
// uses) — but does nothing else. No fade, no callback; presentation-side
// systems that want a fade-out read remainingSeconds themselves.
namespace LifetimeSystem {

inline void update(Registry &registry, float dt)
{
    // Registry::view<T>() materializes a vector before iteration, so destroy()
    // mid-loop here is safe by the same guarantee CollisionSystem relies on.
    for (EntityID id : registry.view<LifetimeComponent>())
    {
        auto *lifetime = registry.get<LifetimeComponent>(id);
        lifetime->remainingSeconds -= dt;
        if (lifetime->remainingSeconds <= 0.f)
            registry.destroy(id);
    }
}

}