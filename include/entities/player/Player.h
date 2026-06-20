#pragma once

#include "entities/Entity.h"

class Player : public Entity {
public:
    explicit Player(std::unique_ptr<IMovementMechanics> movement);
};