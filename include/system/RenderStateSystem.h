#pragma once

#include "render/GridRenderComponent.h"
#include "render/FreeRenderComponent.h"
#include "tmp/movement/PositionComponent.h"

// ---------------------------------------------------------------------------
// RenderStateSystem — free functions holding the logic that used to
// live inside AnimationComponentAdapter/AnimationComponent and
// WalkCycleTimerAdapter/WalkCycleTimer. Those classes are GONE; this is
// where their per-frame stepping logic moved to.
//
// Per the decision to fold render-state stepping directly into
// RenderSystem::render rather than a separate system pass: these
// functions are called FROM RenderSystem::render, once per entity,
// immediately before that entity is drawn. This makes render() impure
// (it mutates GridRenderComponent/FreeRenderComponent's elapsed-time
// and lerp-progress fields as a side effect of drawing) — a conscious
// tradeoff, not an oversight: it avoids a second full entity pass with
// no other caller, at the cost of render() no longer being a pure
// read-and-draw function.
// ---------------------------------------------------------------------------
namespace RenderStateSystem {

// Ported from AnimationComponent::begin()/update(). Detects a change
// in logical position versus what this component last saw (the
// "begin a new lerp" trigger), then advances progress by lerpSpeed*dt,
// clamped at 1.0, lerping renderX/Y from start toward target.
//
// CAVEAT: the original AnimationComponentAdapter class that drove this
// detection was never shared with me — only IRenderState.h's comment
// describing its contract ("on the frame logicalX/Y CHANGES versus
// what it saw last frame, it calls animation_.begin(...)"). This is a
// faithful port of THAT documented contract using
// hasSeenLogicalPosition/lastSeenLogicalX/Y as the change-detection
// mechanism. If the real adapter detected "changed" some other way
// (e.g. comparing against the lerp's own current target instead of
// last frame's logical position), this would need reconciling against
// the actual adapter body once available.
void stepGridRender(GridRenderComponent& render, float dt,
                     float logicalX, float logicalY);

// Ported verbatim from WalkCycleTimer::update()/getProgress(). Echoes
// logical position straight into renderX/Y (no interpolation — free
// movement's position IS already the render position), accumulates
// elapsed time while moving, resets to 0 when stopped.
void stepFreeRender(FreeRenderComponent& render, float dt,
                     float logicalX, float logicalY, bool isMoving);

// Ported verbatim from WalkCycleTimer::getProgress(): elapsed time
// wrapped into a repeating 0..1 value, suitable for
// SpriteAtlas::frameFromProgress. This is a pure read of derived state
// (not stored on the component), same as the original being a
// computed getter rather than a stored field.
float getFreeProgress(const FreeRenderComponent& render);

} // namespace RenderStateSystem