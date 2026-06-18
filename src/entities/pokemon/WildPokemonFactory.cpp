#include "entities/pokemon/PokemonFactory.h"
#include "entities/pokemon/WildPokemon.h"

#include <stdexcept>
#include <string>

WildPokemonFactory::WildPokemonFactory(ISpeciesRepository& repository)
    : repository_(repository)
{
}

Pokemon* WildPokemonFactory::create(int pokedexId, int level) {
    static constexpr int MIN_LEVEL = 1;
    static constexpr int MAX_LEVEL = 100;

    if (level < MIN_LEVEL || level > MAX_LEVEL) {
        throw std::invalid_argument(
            "Invalid level for Pokemon: " + std::to_string(level) +
            ". Must be between 1 and 100."
        );
    }

    if (!repository_.findById(pokedexId)) {
        throw std::invalid_argument(
            "Invalid Pokemon species ID: " + std::to_string(pokedexId)
        );
    }

    return new WildPokemon(pokedexId, level);
}