#pragma once
#include <stdexcept>

// ---------------------------------------------------------------------------
// BaseStats — passive data bundle holding level-1 base values for a species.
// Plain struct: no invariants, no logic, just six integers travelling together.
// ---------------------------------------------------------------------------
struct BaseStats {
    int hit_points;
    int attack;
    int defence;
    int sp_attack;
    int sp_defence;
    int speed;
};

// ---------------------------------------------------------------------------
// Stats — derived, live stats for a CapturedPokemon at a given level.
//
// Invariant: all members are always consistent with the BaseStats + level
// they were computed from. Members are private to protect that invariant.
//
// Scaling formula (linear, Gen-I inspired):
//   HP    = floor(base_hp    * level / 50) + level + 10
//   Other = floor(base_stat  * level / 50) + 5
// ---------------------------------------------------------------------------
class Stats {
public:
    // Compute stats immediately on construction. Throws if level out of range.
    Stats(const BaseStats& base, int level);

    // Recompute in-place (e.g. after levelling up).
    void recompute(const BaseStats& base, int newLevel);

    // Accessors — named with get_ prefix to avoid clashing with member names.
    int get_hp()         const { return hp_; }
    int get_attack()     const { return attack_; }
    int get_defence()    const { return defence_; }
    int get_sp_attack()  const { return sp_attack_; }
    int get_sp_defence() const { return sp_defence_; }
    int get_speed()      const { return speed_; }

private:
    int hp_;
    int attack_;
    int defence_;
    int sp_attack_;
    int sp_defence_;
    int speed_;

    // Core scaling logic. Called by constructor and recompute().
    void compute(const BaseStats& base, int level);

    static constexpr int MIN_LEVEL = 1;
    static constexpr int MAX_LEVEL = 100;
};