#pragma once

// ---------------------------------------------------------------------------
// GridRenderComponent — pure data for the grid-hop visual slide.
//
// REPLACES AnimationComponentAdapter (and, underneath it,
// AnimationComponent) as classes. The logic that used to live in
// AnimationComponent::begin()/update() (detect a position change vs.
// last frame, lerp from old to new over time, clamp progress at 1.0)
// now lives in RenderSystem::stepGridRender() — this struct only holds
// the state that logic reads and writes.
//
// Field-for-field, this is AnimationComponent's old private section:
//   startX_/startY_     -> startX/startY
//   targetX_/targetY_   -> targetX/targetY
//   currentX_/currentY_ -> renderX/renderY
//   progress_           -> progress (1.0 = idle/not animating, matching
//                          AnimationComponent's own isAnimating() ==
//                          progress_ < 1.f convention exactly)
//   speed_              -> lerpSpeed (progress added per second)
//
// progress starts at 1.0 (not 0.0) to match AnimationComponent's
// default-constructed state: a render component that has never seen a
// position change should report "not animating" / "fully settled",
// same as a freshly constructed AnimationComponent did.
// ---------------------------------------------------------------------------
struct GridRenderComponent {
    float startX = 0.f;
    float startY = 0.f;
    float targetX = 0.f;
    float targetY = 0.f;

    // Current interpolated draw position — equals targetX/Y once
    // progress reaches 1.0.
    float renderX = 0.f;
    float renderY = 0.f;

    // 1.0 = idle (matches AnimationComponent::isAnimating() == progress < 1.f).
    float progress = 1.0f;

    // Progress gained per second while animating — same role as
    // AnimationComponent's speed_ constructor parameter (default 6.0f
    // in the original).
    float lerpSpeed = 6.0f;

    // Last logical (gridX, gridY)-derived world position RenderSystem
    // saw, used to detect "position changed since last frame" the same
    // way the old IRenderState::update() comment described — the
    // adapter is gone, so this detection now lives on the component
    // itself instead of inside a wrapper class.
    float lastSeenLogicalX = 0.f;
    float lastSeenLogicalY = 0.f;
    bool hasSeenLogicalPosition = false;
};