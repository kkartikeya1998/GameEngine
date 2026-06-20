#include "render/SpriteAtlas.h"

#include <stdexcept>

SpriteAtlas::SpriteAtlas(const std::string& spritesheet_path)
    : Atlas(spritesheet_path)
{
    const int frameW = 32;
    const int frameH = 48;
    const int playerRowY = frameH * 1;

    loadRegion("player_down_0",    0 * frameW, playerRowY, frameW, frameH);
    loadRegion("player_down_1",    1 * frameW, playerRowY, frameW, frameH);
    loadRegion("player_down_2",    2 * frameW, playerRowY, frameW, frameH);

    loadRegion("player_left_0",  3 * frameW, playerRowY, frameW, frameH);
    loadRegion("player_left_1",  4 * frameW, playerRowY, frameW, frameH);
    loadRegion("player_left_2",  5 * frameW, playerRowY, frameW, frameH);

    loadRegion("player_right_0",  6 * frameW, playerRowY, frameW, frameH);
    loadRegion("player_right_1",  7 * frameW, playerRowY, frameW, frameH);
    loadRegion("player_right_2",  8 * frameW, playerRowY, frameW, frameH);

    loadRegion("player_up_0",  9 * frameW, playerRowY, frameW, frameH);
    loadRegion("player_up_1", 10 * frameW, playerRowY, frameW, frameH);
    loadRegion("player_up_2", 11 * frameW, playerRowY, frameW, frameH);
}

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
    return findRegion(key);
}