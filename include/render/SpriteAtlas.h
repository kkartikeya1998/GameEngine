#pragma once

#include "render/Atlas.h"
#include "entities/movement/Position.h" // for Direction

// ---------------------------------------------------------------------------
// SpriteAtlas — player walk-cycle sprite lookup.
//
// Owns the spritesheet for the player and maps (Direction, WalkFrame) to
// its SpriteRegion. frameFromProgress() converts a continuous animation
// progress [0,1] into a discrete walk-cycle frame.
// ---------------------------------------------------------------------------
enum class WalkFrame {
    Standing,
    StepA,
    StepB
};

class SpriteAtlas : public Atlas {
public:
    explicit SpriteAtlas(const std::string& spritesheet_path);

    const sf::Texture& playerTexture() const { return texture(); }
    SpriteRegion getPlayerSprite(Direction facing, WalkFrame frame) const;

    static WalkFrame frameFromProgress(float p)
    {
        if (p < 0.25f) return WalkFrame::Standing;
        if (p < 0.50f) return WalkFrame::StepA;
        if (p < 0.75f) return WalkFrame::Standing;
        if (p < 1.00f) return WalkFrame::StepB;
        return WalkFrame::Standing;
    }
};