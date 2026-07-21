#pragma once

#include <string>
#include "component/DirectionComponent.h"
#include "component/MovementStateComponent.h"

inline std::string toString(Direction direction) {
    switch (direction) {
        case Direction::UP:    return "up";
        case Direction::UP_RIGHT: return "up_right";
        case Direction::RIGHT: return "right";
        case Direction::DOWN_RIGHT: return "down_right";
        case Direction::DOWN:  return "down";
        case Direction::DOWN_LEFT: return "down_left";
        case Direction::LEFT:  return "left";
        case Direction::UP_LEFT: return "up_left";
        case Direction::NONE:  return "down"; // fallback facing
    }
    return "down";
}

inline std::string toString(MovementState state) {
    switch (state) {
        case MovementState::Idle:      return "idle";
        case MovementState::Walking:   return "walk";
        case MovementState::Sprinting: return "run";
        case MovementState::Jumping:   return "jump";
    }
    return "walk";
}