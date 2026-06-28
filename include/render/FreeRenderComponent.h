#pragma once

// ---------------------------------------------------------------------------
// FreeRenderComponent — pure data for the free-movement walk cycle.
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
struct FreeRenderComponent {
    float cyclesPerSecond = 2.0f;
    float elapsed = 0.f;

    // Echoed straight from FreeMovementComponent's x/y each frame — no
    // interpolation for free movement, same as
    // WalkCycleTimerAdapter::getRenderX/Y just returning lastLogicalX/Y.
    float renderX = 0.f;
    float renderY = 0.f;
};