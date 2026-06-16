#pragma once
#include <string_view>

namespace Terrain {
enum class Type {
    Grass,
    Sand,
    Water,
    Ice,
    Cave,
    Building
};

constexpr bool isWalkableBase(Type type) {
    switch (type) {
        case Type::Grass:
        case Type::Sand:
        case Type::Cave:
        case Type::Building:
            return true;
        case Type::Water:
        case Type::Ice:
            return false;
    }
    return false;
}

constexpr std::string_view toString(Type type) {
    switch (type) {
        case Type::Grass:    return "G";
        case Type::Sand:     return "S";
        case Type::Water:    return "W";
        case Type::Ice:      return "I";
        case Type::Cave:     return "C";
        case Type::Building: return "B";
    }
    return "?";
}
}