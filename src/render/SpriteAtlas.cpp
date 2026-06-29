#include "render/SpriteAtlas.h"
#include <stdexcept>

SpriteAtlas::SpriteAtlas(const std::filesystem::path& spritesheet_path, const SpriteRepository& spriteRepository)
    : Atlas(spritesheet_path)
    , spriteRepository_(spriteRepository)
{}

static std::string dirToString(Direction facing)
{
    switch (facing) {
        case Direction::UP: return "up";
        case Direction::DOWN: return "down";
        case Direction::LEFT: return "left";
        case Direction::RIGHT: return "right";
        default: return "down";
    }
}

SpriteRegion SpriteAtlas::getPlayerSprite(Direction facing, WalkFrame frame) const
{
    std::string dir = dirToString(facing);
    int idx = 0;

    switch (frame) {
        case WalkFrame::Standing: idx = 0; break;
        case WalkFrame::StepA:    idx = 1; break;
        case WalkFrame::StepB:    idx = 2; break;
    }

    std::string key = "player_" + dir + "_" + std::to_string(idx);

    return regionFromRepository(spriteRepository_, key, key);
}