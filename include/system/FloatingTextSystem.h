#pragma once
#include <algorithm>
#include "entities/Registry.h"
#include "component/FloatingTextComponent.h"
#include "component/PositionComponent.h"
#include "component/TextRenderComponent.h"
#include "component/LifetimeComponent.h"

// Drives popup text (damage numbers, "+N HP", etc.): follows its owner's
// current position, drifts upward over its lifetime, and fades out as
// LifetimeComponent::remainingSeconds runs down. Reads LifetimeComponent
// but never writes it — LifetimeSystem still owns destruction.
namespace FloatingTextSystem {

inline void update(Registry &registry)
{
    for (EntityID id : registry.view<FloatingTextComponent, PositionComponent, TextRenderComponent, LifetimeComponent>())
    {
        auto *floating = registry.get<FloatingTextComponent>(id);
        auto *pos = registry.get<PositionComponent>(id);
        auto *text = registry.get<TextRenderComponent>(id);
        auto *lifetime = registry.get<LifetimeComponent>(id);

        auto *ownerPos = registry.get<PositionComponent>(floating->owner);
        if (!ownerPos)
            continue; // owner gone — leave the popup at its last position rather than guessing

        float elapsed = std::max(0.f, floating->totalSeconds - lifetime->remainingSeconds);

        pos->x = ownerPos->x + floating->baseOffset.x;
        pos->y = ownerPos->y + floating->baseOffset.y - (elapsed * floating->riseSpeed);

        float fadeFraction = std::clamp(lifetime->remainingSeconds / floating->totalSeconds, 0.f, 1.f);
        text->color.a = static_cast<std::uint8_t>(255.f * fadeFraction);
    }
}

}