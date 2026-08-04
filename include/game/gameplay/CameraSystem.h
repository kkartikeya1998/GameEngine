#pragma once

#include "game/ecs/movement/PositionComponent.h"
#include "game/world/Map.h"
#include "game/ecs/tbd/CameraComponent.h"

namespace CameraSystem {

void update(const PositionComponent& playerPos, const Map& map, CameraComponent& camera);

}