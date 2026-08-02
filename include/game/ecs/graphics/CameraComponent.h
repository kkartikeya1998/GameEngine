#pragma once

#include "engine/ecs/Registry.h"
#include "game/GameConstants.h"

// ---------------------------------------------------------------------------
// CameraComponent — gameplay camera state, lives on the camera entity owned
// by GameplayState's World. CameraSystem writes centerX/centerY each frame
// from the follow target's position; GameplayState reads this component to
// build the CameraView it hands to RenderSystem.
// ---------------------------------------------------------------------------
struct CameraComponent {
    float centerX = 0.f;
    float centerY = 0.f;

    static constexpr int kVisibleTilesX = GameConstants::TILES_VISIBLE_X;
    float viewWidth = kVisibleTilesX * GameConstants::TILE_SIZE;
    float viewHeight = viewWidth *
        (static_cast<float>(GameConstants::GAME_RESOLUTION_H) / GameConstants::GAME_RESOLUTION_W);

    EntityID followTarget{};
    bool active = true;
};