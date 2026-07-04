#pragma once

#include <string>
#include "tmp/movement/DirectionComponent.h"
#include "tmp/movement/MovementStateComponent.h"

inline std::string toString(Direction direction) {
    switch (direction) {
        case Direction::UP:    return "up";
        case Direction::DOWN:  return "down";
        case Direction::LEFT:  return "left";
        case Direction::RIGHT: return "right";
        case Direction::NONE:  return "down"; // fallback facing
    }
    return "down";
}

inline std::string toString(MovementState state) {
    switch (state) {
        case MovementState::Idle:      return "walk"; // idle reuses walk frame 0
        case MovementState::Walking:   return "walk";
        case MovementState::Sprinting: return "run";
    }
    return "walk";
}