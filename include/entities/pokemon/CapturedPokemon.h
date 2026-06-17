#pragma once

#include "entities/pokemon/Pokemon.h"
#include <string>

class CapturedPokemon : public Pokemon {
private:
    std::string nickname_;
public:
    CapturedPokemon(int species_id, int level, const std::string& nickname);

    const std::string& nickname() const;
    void set_nickname(const std::string& nickname);
};