#pragma once

#include "system/GameConstants.h"

// ---------------------------------------------------------------------------
// Camera — World-space center point the renderer should
// view around, plus the viewport size in pixels. 
// Not a Component - there is exactly one of these per gameplay session, owned by
// GameplayState, not per-entity data living in World.
// ---------------------------------------------------------------------------
struct Camera {
    float centerX = 0.f;
    float centerY = 0.f;

    static constexpr int kVisibleTilesX = GameConstants::TILES_VISIBLE_X;
    float viewWidth  = kVisibleTilesX * GameConstants::TILE_SIZE;
    float viewHeight = viewWidth *
        (static_cast<float>(GameConstants::GAME_RESOLUTION_H) / GameConstants::GAME_RESOLUTION_W);
};