#include "entities/pokemon/PokemonFactory.h"
#include "entities/pokemon/CapturedPokemon.h"

#include <stdexcept>
#include <string>

CapturedPokemonFactory::CapturedPokemonFactory(ISpeciesRepository& repository)
    : repository_(repository)
{
}

const std::string& CapturedPokemonFactory::get_nickname_for(int pokedexId, int level) {
    // For now, return a simple default nickname.
    // [TODO] Implement nicknaming using player input 
    static std::string default_nickname = "Player";
    return default_nickname;
}

Pokemon* CapturedPokemonFactory::create(int pokedexId, int level) {
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

    const std::string& nickname = get_nickname_for(pokedexId, level);

    return new CapturedPokemon(pokedexId, level, nickname);
}