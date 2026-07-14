#pragma once
#include "component/DirectionComponent.h"
#include "entities/Component.h"

struct PlayerControlComponent : public Component {
    Direction direction = Direction::NONE;
    bool sprinting = false;
    bool jumpRequested = false;
};