#include "entities/pokemon/PokemonSpeciesRepository.h"
#include "pokemon/poke_species_gen1.h"

#include <stdexcept>

PokemonSpeciesRepository& PokemonSpeciesRepository::instance() {
    static PokemonSpeciesRepository repo;
    return repo;
}

PokemonSpeciesRepository::PokemonSpeciesRepository() {
    load_gen1();
}

void PokemonSpeciesRepository::load_gen1() {
    species_by_id_.reserve(GEN1_SPECIES_COUNT);
    species_by_name_.reserve(GEN1_SPECIES_COUNT);

    for (std::size_t i = 0; i < GEN1_SPECIES_COUNT; ++i) {
        const PokemonSpecies species(GEN1_SPECIES[i]);

        // Use insert/emplace instead of operator[]
        species_by_id_.insert({species.pokedexId, species});
        species_by_name_.insert({species.name, &species_by_id_.at(species.pokedexId)});
    }
}

const PokemonSpecies* PokemonSpeciesRepository::findByName(const std::string& name) const {
    const auto it = species_by_name_.find(name);
    if (it == species_by_name_.end()) {
        return nullptr;
    }
    return it->second;
}

const PokemonSpecies* PokemonSpeciesRepository::findById(int pokedexId) const {
    const auto it = species_by_id_.find(pokedexId);
    if (it == species_by_id_.end()) {
        return nullptr;
    }
    return &it->second;
}

std::vector<const PokemonSpecies*> PokemonSpeciesRepository::getAll() const {
    std::vector<const PokemonSpecies*> result;
    result.reserve(species_by_id_.size());

    for (const auto& pair : species_by_id_) {
        result.push_back(&pair.second);
    }

    return result;
}

std::size_t PokemonSpeciesRepository::count() const {
    return species_by_id_.size();
}