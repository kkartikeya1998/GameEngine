#pragma once
#include <string_view>

namespace Terrain {
enum class Type {
    Grass,
    Sand,
    Water,
    Ice,
    Cave,
    Building,
    Tree
};

static Type terrain_from_string(const std::string& s)
{
    if (s == "Grass")   return Type::Grass;
    if (s == "Water")   return Terrain::Type::Water;
    if (s == "Sand")    return Terrain::Type::Sand;
    if (s == "Ice")     return Terrain::Type::Ice;
    if (s == "Cave")    return Terrain::Type::Cave;
    if (s == "Building") return Terrain::Type::Building;
    if(s == "Tree")     return Terrain::Type::Tree;
    // etc.

    return Terrain::Type::Grass; // default
}

constexpr bool isWalkableBase(Type type) {
    switch (type) {
        case Type::Grass:
        case Type::Sand:
        case Type::Cave:
        case Type::Building:
            return true;
        case Type::Water:
        case Type::Ice:
        case Type::Tree:
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
        case Type::Tree:     return "T";
    }
    return "?";
}
}