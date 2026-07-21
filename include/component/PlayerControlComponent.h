#pragma once

#include <SFML/System/Vector2.hpp>
#include "entities/Component.h"

struct PlayerControlComponent : public Component
{
    sf::Vector2f move{0.f, 0.f};

    bool sprinting = false;
    bool jumpRequested = false;
    bool interactRequested = false;
};