#pragma once

#include "entities/pokemon/Pokemon.h"

class WildPokemon : public Pokemon {
public:
    WildPokemon(int species_id, int level);

    bool capture();
};