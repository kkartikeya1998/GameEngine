#pragma once

#include <unordered_map>

#include "entities/pokemon/Pokemon.h"
#include "entities/pokemon/PokemonSpeciesRepository.h"

class PokemonFactory {
public:
    virtual ~PokemonFactory() = default;

    // Factory method to create a Pokemon instance given a species ID and level.
    // Throws if species ID is invalid or level out of range.  
    virtual Pokemon* create(int pokedexId, int level) = 0;
};


class WildPokemonFactory : public PokemonFactory {
public:
    explicit WildPokemonFactory(ISpeciesRepository& repository);

    Pokemon* create(int pokedexId, int level) override;

private:
    ISpeciesRepository& repository_;
};

class CapturedPokemonFactory : public PokemonFactory {
public:
    explicit CapturedPokemonFactory(ISpeciesRepository& repository);

    Pokemon* create(int pokedexId, int level) override;

private:
    const std::string& get_nickname_for(int pokedexId, int level);

    ISpeciesRepository& repository_;
};