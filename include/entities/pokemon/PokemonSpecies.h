#pragma once
#include <string>
#include <optional>
#include "Stats.h"

// ---------------------------------------------------------------------------
// EvolutionTrigger / EvolutionCondition
// ---------------------------------------------------------------------------
enum class EvolutionTrigger {
    LEVEL,       // evolves at a specific level
    ITEM,        // evolves when a specific item is used
    FRIENDSHIP,  // evolves when friendship reaches threshold
    TRADE        // evolves when traded
};

struct EvolutionCondition {
    EvolutionTrigger trigger;
    int              parameter;   // level value, item ID, friendship threshold
    int              evolvesInto; // pokedex ID of the target species
};

// ---------------------------------------------------------------------------
// SpeciesData — plain-old-data struct suitable for constexpr storage.
//
// Uses const char* instead of std::string (std::string is not literal type),
// and a sentinel-based optional instead of std::optional<EvolutionCondition>
// (std::optional is not constexpr-constructible in C++17).
//
// This is the compile-time representation. It lives in .rodata.
// ---------------------------------------------------------------------------
struct SpeciesData {
    int              pokedexId;
    const char*      name;
    const char*      type1;
    const char*      type2;       // nullptr if single-type
    BaseStats        baseStats;
    bool             hasEvolution;
    EvolutionCondition evolution; // only valid when hasEvolution == true
};

// ---------------------------------------------------------------------------
// PokemonSpecies — runtime-facing type used throughout the game.
//
// Constructed from SpeciesData once at repository init time. Uses std::string
// and std::optional for convenience. All access via const pointer from repo.
// ---------------------------------------------------------------------------
struct PokemonSpecies {
    int                               pokedexId;
    std::string                       name;
    std::string                       type1;
    std::string                       type2;      // empty string if single-type
    BaseStats                         baseStats;
    std::optional<EvolutionCondition> evolution;  // nullopt if does not evolve

    // Construct from the POD compile-time record.
    explicit PokemonSpecies(const SpeciesData& d)
        : pokedexId(d.pokedexId)
        , name(d.name)
        , type1(d.type1)
        , type2(d.type2 ? d.type2 : "")
        , baseStats(d.baseStats)
        , evolution(d.hasEvolution ? std::optional<EvolutionCondition>(d.evolution)
                                   : std::nullopt)
    {}
};