#include "entities/pokemon/Pokemon.h"

Pokemon::Pokemon(int species_id, int level)
    : species_id_(species_id), level_(level) {}

int Pokemon::species_id() const {
    return species_id_;
}

int Pokemon::level() const {
    return level_;
}