#pragma once
#include <string>
#include <vector>
#include <optional>

#include "PokemonSpecies.h"

// ---------------------------------------------------------------------------
// ISpeciesRepository
//
// Repository pattern: abstracts the data source for PokemonSpecies entirely.
// Callers (EncounterSystem, PokemonFactory, etc.) depend only on this
// interface — they never know whether data comes from JSON files, a database,
// a network API, or a hardcoded test stub.
//
// Ownership contract:
//   - The repository owns all PokemonSpecies objects.
//   - All returned pointers are non-owning (const raw pointers).
//   - Callers must not delete or store owning references to returned pointers.
//   - Pointers remain valid for the lifetime of the repository instance.
// ---------------------------------------------------------------------------
class ISpeciesRepository {
public:
    virtual ~ISpeciesRepository() = default;

    // Returns a non-owning pointer, or nullptr if not found.
    virtual const PokemonSpecies* findByName(const std::string& name) const = 0;

    // Returns a non-owning pointer, or nullptr if not found.
    virtual const PokemonSpecies* findById(int pokedexId) const = 0;

    // Returns non-owning pointers to all loaded species.
    virtual std::vector<const PokemonSpecies*> getAll() const = 0;

    // Returns total number of species loaded.
    virtual std::size_t count() const = 0;
};