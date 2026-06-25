#pragma once

#include "entities/FreeEntity.h"

class Player : public FreeEntity {
public:
    explicit Player(std::unique_ptr<IFreeMovementMechanics> movement,
                    float walkCyclePerSeconds = 4.0f);
};