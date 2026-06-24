#pragma once
#include <string_view>

namespace Terrain {
enum class Type {
    Grass1,
    Grass2,
    Grass3,
    Grass4,
    Sand,
    Water,
    Ice,
    Cave,
    Building,
    Tree
};

static Type terrain_from_string(const std::string& s)
{
    if (s == "Grass1")   return Type::Grass1;
    if (s == "Grass2")   return Type::Grass2;
    if (s == "Grass3")   return Type::Grass3;
    if (s == "Grass4")   return Type::Grass4;
    if (s == "Water")   return Terrain::Type::Water;
    if (s == "Sand")    return Terrain::Type::Sand;
    if (s == "Ice")     return Terrain::Type::Ice;
    if (s == "Cave")    return Terrain::Type::Cave;
    if (s == "Building") return Terrain::Type::Building;
    if(s == "Tree")     return Terrain::Type::Tree;
    // etc.

    return Terrain::Type::Grass1; // default
}

constexpr bool isWalkableBase(Type type) {
    switch (type) {
        case Type::Grass1:
        case Type::Grass2:
        case Type::Grass3:
        case Type::Grass4:
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
        case Type::Grass1:    return "Grass1";
        case Type::Grass2:    return "Grass2";
        case Type::Grass3:    return "Grass3";
        case Type::Grass4:    return "Grass4";
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