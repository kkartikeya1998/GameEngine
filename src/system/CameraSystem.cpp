#include "system/CameraSystem.h"
#include "system/GameConstants.h"

#include <algorithm>

void CameraSystem::update(const PositionComponent& playerPos, const Map& map, Camera& camera) {
    float mapPixelW = static_cast<float>(map.getWidth())  * GameConstants::TILE_SIZE;
    float mapPixelH = static_cast<float>(map.getHeight()) * GameConstants::TILE_SIZE;

    float halfW = camera.viewWidth  / 2.f;
    float halfH = camera.viewHeight / 2.f;

    // If the map is smaller than the viewport in a given axis, center
    // on the map itself instead of the player, so we never try to
    // clamp into an inverted (min > max) range.
    camera.centerX = (mapPixelW <= camera.viewWidth)
        ? mapPixelW / 2.f
        : std::clamp(playerPos.x, halfW, mapPixelW - halfW);

    camera.centerY = (mapPixelH <= camera.viewHeight)
        ? mapPixelH / 2.f
        : std::clamp(playerPos.y, halfH, mapPixelH - halfH);
}