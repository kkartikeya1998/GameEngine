#pragma once
#include <cstdint>

// Bitmask of movement modes a tile permits, or an entity is capable of.
// Not mutually exclusive: a tile (or entity) can support several at once.
enum class MovementMode : uint8_t {
    None    = 0,
    Walking = 1 << 0,
    Surfing = 1 << 1,
    Flying  = 1 << 2,
};

constexpr MovementMode operator|(MovementMode a, MovementMode b) {
    return static_cast<MovementMode>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
constexpr MovementMode operator&(MovementMode a, MovementMode b) {
    return static_cast<MovementMode>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}
constexpr bool any(MovementMode m) { return static_cast<uint8_t>(m) != 0; }