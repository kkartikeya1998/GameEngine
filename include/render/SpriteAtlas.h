#pragma once

#include "render/Atlas.h"
#include "entities/movement/Position.h" // for Direction
#include "world/SpriteRepository.h"

// ---------------------------------------------------------------------------
// SpriteAtlas — player walk-cycle sprite lookup.
//
// ---------------------------------------------------------------------------
enum class WalkFrame {
    Standing,
    StepA,
    StepB
};

class SpriteAtlas : public Atlas {
public:
    SpriteAtlas(const std::string& spritesheet_path, const SpriteRepository& spriteRepository);

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

private:
    const SpriteRepository& spriteRepository_; // non-owning, repo outlives atlas
};