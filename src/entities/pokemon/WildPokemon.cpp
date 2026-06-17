#include "entities/pokemon/WildPokemon.h"
#include "entities/pokemon/CapturedPokemon.h"

#include <string>

WildPokemon::WildPokemon(int species_id, int level)
    : Pokemon(species_id, level)
{
}

bool WildPokemon::capture() {
    // For now, capture is always successful.
    // [TODO] Implement actual capture mechanics based on species, level, HP, etc.
    return true;
}