#pragma once
#include "entities/Component.h"

// Marks an overworld entity as a wild Pokemon encounter. Deliberately
// just id+level — never a Pokemon/WildPokemon instance. The real
// domain object is built via WildPokemonFactory::create() only when a
// battle actually starts; this component and that object never
// coexist, so there's exactly one place species/level data lives
// (PokemonSpeciesRepository) rather than two.
struct WildSpawnComponent : public Component {
    int speciesId = 0;
    int level = 1;

    WildSpawnComponent() = default;
    WildSpawnComponent(int speciesId, int level)
        : speciesId(speciesId), level(level) {}
};