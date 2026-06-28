#include "system/RenderStateSystem.h"

namespace RenderStateSystem {

void stepGridRender(GridRenderComponent& render, float dt,
                     float logicalX, float logicalY)
{
    // Detect a position change vs. last frame — equivalent to the old
    // adapter calling animation_.begin(lastSeenX, lastSeenY, logicalX,
    // logicalY). First frame ever (hasSeenLogicalPosition == false)
    // intentionally does NOT trigger a lerp — there is no "last" yet,
    // so the entity should just appear at its spawn position, matching
    // AnimationComponent's default-constructed progress_ == 1.0 (idle).
    if (render.hasSeenLogicalPosition &&
        (logicalX != render.lastSeenLogicalX || logicalY != render.lastSeenLogicalY))
    {
        // Ported from AnimationComponent::begin(): start the lerp from
        // wherever the render position currently is (NOT from
        // lastSeenLogicalX/Y) — this matters if a previous lerp was
        // interrupted mid-flight by a new hop, same as the original
        // begin() always lerping from the CURRENT render position, not
        // assuming the previous lerp had finished.
        render.startX = render.renderX;
        render.startY = render.renderY;
        render.targetX = logicalX;
        render.targetY = logicalY;
        render.progress = 0.f;
    }

    render.lastSeenLogicalX = logicalX;
    render.lastSeenLogicalY = logicalY;
    render.hasSeenLogicalPosition = true;

    // Ported verbatim from AnimationComponent::update().
    if (render.progress < 1.f) {
        render.progress += render.lerpSpeed * dt;
        if (render.progress > 1.f) {
            render.progress = 1.f;
        }
        render.renderX = render.startX + (render.targetX - render.startX) * render.progress;
        render.renderY = render.startY + (render.targetY - render.startY) * render.progress;
    }
    // When progress is already >= 1.f, renderX/Y are left as-is —
    // matches AnimationComponent::update()'s early return when
    // progress_ >= 1.f (no recomputation once settled).
}

void stepFreeRender(FreeRenderComponent& render, float dt,
                     float logicalX, float logicalY, bool isMoving)
{
    // No interpolation for free movement — logical position IS the
    // render position, same as WalkCycleTimerAdapter::getRenderX/Y
    // just echoing lastLogicalX/Y straight through.
    render.renderX = logicalX;
    render.renderY = logicalY;

    // Ported verbatim from WalkCycleTimer::update().
    if (isMoving) {
        render.elapsed += dt;
    } else {
        render.elapsed = 0.f; // reset to standing frame when stopped
    }
}

float getFreeProgress(const FreeRenderComponent& render)
{
    float cycle = render.elapsed * render.cyclesPerSecond;
    return cycle - static_cast<float>(static_cast<int>(cycle));
}

} // namespace RenderStateSystem