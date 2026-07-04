#pragma once

#include "render/atlases/Atlas.h"
#include "asset/repositories/CharacterRepository.h"
// for direction only
#include "tmp/movement/DirectionComponent.h"

// ---------------------------------------------------------------------------
// SpriteAtlas — player walk-cycle sprite lookup.
//
// getCharacterSprite's only job now is building the frame-name key. The
// repository lookup, null-check, and SpriteRegion construction live in
// Atlas::regionFromRepository — see Atlas.h.
// ---------------------------------------------------------------------------


class SpriteAtlas : public Atlas {
public:
    SpriteAtlas(const std::filesystem::path& spritesheet_path, const CharacterRepository& characterRepository);

    const sf::Texture& playerTexture() const { return texture(); }
    SpriteRegion getCharacterSprite(const std::string& spriteName) const;

private:
    const CharacterRepository& characterRepository_;
};