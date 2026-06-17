#include "entities/pokemon/Stats.h"

// ---------------------------------------------------------------------------
// Stats — implementation
// ---------------------------------------------------------------------------

Stats::Stats(const BaseStats& base, int level) {
    compute(base, level);
}

void Stats::recompute(const BaseStats& base, int newLevel) {
    compute(base, newLevel);
}

void Stats::compute(const BaseStats& base, int level) {
    if (level < MIN_LEVEL || level > MAX_LEVEL) {
        throw std::out_of_range("Level must be between 1 and 100");
    }

    // HP has a different formula to all other stats.
    hp_        = (base.hit_points * level / 50) + level + 10;

    // All other stats share the same formula.
    attack_    = (base.attack    * level / 50) + 5;
    defence_   = (base.defence   * level / 50) + 5;
    sp_attack_ = (base.sp_attack * level / 50) + 5;
    sp_defence_= (base.sp_defence* level / 50) + 5;
    speed_     = (base.speed     * level / 50) + 5;
}