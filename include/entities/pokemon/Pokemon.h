#pragma once

#include <string>
#include "PokemonSpecies.h"

class Pokemon {
public:
    Pokemon(int species_id, int level);
    virtual ~Pokemon() = default;

    int species_id() const;
    int level() const;

protected:
    int species_id_;
    int level_;
};