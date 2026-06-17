#include "entities/pokemon/CapturedPokemon.h"

CapturedPokemon::CapturedPokemon(
    int species_id,
    int level,
    const std::string& nickname
)
    : Pokemon(species_id, level)
    , nickname_(nickname) {}

const std::string& CapturedPokemon::nickname() const {
    return nickname_;
}

void CapturedPokemon::set_nickname(const std::string& nickname) {
    nickname_ = nickname;
}