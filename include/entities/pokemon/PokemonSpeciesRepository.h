#pragma once

#include "entities/pokemon/ISpeciesRepository.h"
#include "entities/pokemon/PokemonSpecies.h"

#include <unordered_map>
#include <vector>
#include <string>

class PokemonSpeciesRepository : public ISpeciesRepository {
public:
    static PokemonSpeciesRepository& instance();

    const PokemonSpecies* findByName(const std::string& name) const override;
    const PokemonSpecies* findById(int pokedexId) const override;
    std::vector<const PokemonSpecies*> getAll() const override;
    std::size_t count() const override;

private:
    PokemonSpeciesRepository();

    void load_gen1();

    std::unordered_map<int, PokemonSpecies> species_by_id_;
    std::unordered_map<std::string, const PokemonSpecies*> species_by_name_;
};