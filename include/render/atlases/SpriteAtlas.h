#pragma once

#include "render/atlases/Atlas.h"
#include "asset/repositories/CharacterRepository.h"
// for direction only
#include "tmp/movement/DirectionComponent.h"

// ---------------------------------------------------------------------------
// SpriteAtlas — player walk-cycle sprite lookup.
//
// getPlayerSprite's only job now is building the frame-name key. The
// repository lookup, null-check, and SpriteRegion construction live in
// Atlas::regionFromRepository — see Atlas.h.
// ---------------------------------------------------------------------------
enum class WalkFrame {
    Standing,
    StepA,
    StepB
};

class SpriteAtlas : public Atlas {
public:
    SpriteAtlas(const std::filesystem::path& spritesheet_path, const CharacterRepository& characterRepository);

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
    const CharacterRepository& characterRepository_;
};