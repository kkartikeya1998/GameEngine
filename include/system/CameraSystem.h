#pragma once

#include "tmp/component/PositionComponent.h"
#include "world/Map.h"
#include "render/Camera.h"

// ---------------------------------------------------------------------------
// CameraSystem — stateless free function, same shape as MovementSystem.
// Computes where the camera should center this frame given the
// player's position and the active map's bounds, clamping so the
// viewport never shows past the map edges.
// ---------------------------------------------------------------------------
namespace CameraSystem {

void update(const PositionComponent& playerPos, const Map& map, Camera& camera);

}