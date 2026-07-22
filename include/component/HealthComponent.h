#pragma once
#include <algorithm>
#include "entities/Component.h"

// Authoritative HP for any entity that can take damage or be healed.
// currentHP is the single source of truth — presentation (health bar fill,
// drain animation) reads from this but never writes it.
struct HealthComponent : public Component {
    int currentHP = 0;
    int maxHP = 0;

    HealthComponent() = default;
    HealthComponent(int currentHP, int maxHP) : currentHP(currentHP), maxHP(maxHP) {}

    void Heal(int amount)
    {
        currentHP = std::min(currentHP + amount, maxHP);
    }

    void Damage(int amount)
    {
        currentHP = std::max(currentHP - amount, 0);
    }
};