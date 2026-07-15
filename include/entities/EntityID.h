#pragma once
#include <cstdint>
#include <functional>

struct EntityID {
    uint32_t index = 0;
    uint32_t generation = 0;

    bool operator==(const EntityID& o) const {
        return index == o.index && generation == o.generation;
    }
};

struct EntityIDHash {
    std::size_t operator()(const EntityID& id) const {
        return std::hash<uint64_t>()(
            (static_cast<uint64_t>(id.index) << 32) | id.generation);
    }
};

inline constexpr EntityID NullEntity{0xFFFFFFFFu, 0};