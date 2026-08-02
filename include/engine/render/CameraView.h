#pragma once

// ---------------------------------------------------------------------------
// CameraView — pure rendering data: world-space center point plus viewport
// size in pixels. Engine-owned, carries no gameplay semantics (no follow
// target, no zoom, no active flag). This is the only "camera" type
// RenderSystem is allowed to know about.
// ---------------------------------------------------------------------------
struct CameraView {
    float centerX = 0.f;
    float centerY = 0.f;
    float viewWidth = 0.f;
    float viewHeight = 0.f;
};