#pragma once

#include "entities/Component.h"

// ---------------------------------------------------------------------------
// FreeRenderComponent — pure data for the free-movement walk cycle.
//
// CHANGED (Component base pass): now inherits `: public Component` so
// it can live in Entity's vector<unique_ptr<Component>> storage. No
// field changes — see Component.h for why this base exists.
//
// REPLACES WalkCycleTimerAdapter (and, underneath it, WalkCycleTimer) as
// classes. The logic that used to live in WalkCycleTimer::update()/
// getProgress() (accumulate elapsed time while moving, reset to 0 when
// stopped, wrap elapsed*cyclesPerSecond into a repeating 0..1 value) now
// lives in RenderSystem::stepFreeRender() — this struct only holds the
// state that logic reads and writes.
//
// Field-for-field, this is WalkCycleTimer's old private section:
//   cyclesPerSecond_ -> cyclesPerSecond
//   elapsed_         -> elapsed
//
// renderX/renderY are new relative to WalkCycleTimer itself (which had
// no position concept at all) but match what WalkCycleTimerAdapter did
// around it: free movement's logical position IS the render position,
// no interpolation, so these just echo the logical x/y the system was
// last given.
// ---------------------------------------------------------------------------
// CAVEAT (Component base pass, confirmed by Player.h's build failure):
// same issue as FreeMovementComponent — Component's virtual destructor
// disqualifies this struct from aggregate-init, and Player.h's
// makePlayer() brace-initializes it with 4 positional args. Added the
// constructor below so that call site becomes a normal constructor
// call instead.
struct FreeRenderComponent : public Component {
    float cyclesPerSecond = 2.0f;
    float elapsed = 0.f;

    // Echoed straight from FreeMovementComponent's x/y each frame — no
    // interpolation for free movement, same as
    // WalkCycleTimerAdapter::getRenderX/Y just returning lastLogicalX/Y.
    float renderX = 0.f;
    float renderY = 0.f;

    FreeRenderComponent() = default;

    FreeRenderComponent(float cyclesPerSecond, float elapsed, float renderX, float renderY)
        : cyclesPerSecond(cyclesPerSecond)
        , elapsed(elapsed)
        , renderX(renderX)
        , renderY(renderY)
    {}
};