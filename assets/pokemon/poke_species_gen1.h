#pragma once
#include "entities/pokemon/PokemonSpecies.h"

// ---------------------------------------------------------------------------
// Gen1Data — compile-time species table for Generation I.
//
// Data lives entirely in .rodata. Zero heap allocation, zero file I/O,
// zero parse cost. The compiler validates every field against SpeciesData.
//
// Macro helpers reduce repetition and keep columns aligned:
//   NO_EVO          — species that do not evolve
//   EVO(t,p,into)   — species with an evolution condition
// ---------------------------------------------------------------------------

#define NO_EVO        false, {}
#define EVO(t,p,into) true,  EvolutionCondition{ EvolutionTrigger::t, p, into }

inline constexpr SpeciesData GEN1_SPECIES[] = {
//  { id,  name,           type1,     type2,      { hp, atk, def, spa, spd, spe },  evolution         }
    {   1, "Bulbasaur",    "Grass",   "Poison",   { 45,  49,  49,  65,  65,  45 },  EVO(LEVEL, 16,  2) },
    {   2, "Ivysaur",      "Grass",   "Poison",   { 60,  62,  63,  80,  80,  60 },  EVO(LEVEL, 32,  3) },
    {   3, "Venusaur",     "Grass",   "Poison",   { 80,  82,  83, 100, 100,  80 },  NO_EVO             },
    {   4, "Charmander",   "Fire",    nullptr,    { 39,  52,  43,  60,  50,  65 },  EVO(LEVEL, 16,  5) },
    {   5, "Charmeleon",   "Fire",    nullptr,    { 58,  64,  58,  80,  65,  80 },  EVO(LEVEL, 36,  6) },
    {   6, "Charizard",    "Fire",    "Flying",   { 78,  84,  78, 109,  85, 100 },  NO_EVO             },
    {   7, "Squirtle",     "Water",   nullptr,    { 44,  48,  65,  50,  64,  43 },  EVO(LEVEL, 16,  8) },
    {   8, "Wartortle",    "Water",   nullptr,    { 59,  63,  80,  65,  80,  58 },  EVO(LEVEL, 36,  9) },
    {   9, "Blastoise",    "Water",   nullptr,    { 79,  83, 100,  85, 105,  78 },  NO_EVO             },
    {  10, "Caterpie",     "Bug",     nullptr,    { 45,  30,  35,  20,  20,  45 },  EVO(LEVEL,  7, 11) },
    {  11, "Metapod",      "Bug",     nullptr,    { 50,  20,  55,  25,  25,  30 },  EVO(LEVEL, 10, 12) },
    {  12, "Butterfree",   "Bug",     "Flying",   { 60,  45,  50,  90,  80,  70 },  NO_EVO             },
    {  13, "Weedle",       "Bug",     "Poison",   { 40,  35,  30,  20,  20,  50 },  EVO(LEVEL,  7, 14) },
    {  14, "Kakuna",       "Bug",     "Poison",   { 45,  25,  50,  25,  25,  35 },  EVO(LEVEL, 10, 15) },
    {  15, "Beedrill",     "Bug",     "Poison",   { 65,  90,  40,  45,  80,  75 },  NO_EVO             },
    {  16, "Pidgey",       "Normal",  "Flying",   { 40,  45,  40,  35,  35,  56 },  EVO(LEVEL, 18, 17) },
    {  17, "Pidgeotto",    "Normal",  "Flying",   { 63,  60,  55,  50,  50,  71 },  EVO(LEVEL, 36, 18) },
    {  18, "Pidgeot",      "Normal",  "Flying",   { 83,  80,  75,  70,  70, 101 },  NO_EVO             },
    {  19, "Rattata",      "Normal",  nullptr,    { 30,  56,  35,  25,  35,  72 },  EVO(LEVEL, 20, 20) },
    {  20, "Raticate",     "Normal",  nullptr,    { 55,  81,  60,  50,  70,  97 },  NO_EVO             },
    {  25, "Pikachu",      "Electric",nullptr,    { 35,  55,  40,  50,  50,  90 },  EVO(ITEM,  83, 26) },
    {  26, "Raichu",       "Electric",nullptr,    { 60,  90,  55,  90,  80, 110 },  NO_EVO             },
    {  39, "Jigglypuff",   "Normal",  "Fairy",    {115,  45,  20,  45,  25,  20 },  EVO(ITEM,  82, 40) },
    {  40, "Wigglytuff",   "Normal",  "Fairy",    {140,  70,  45,  85,  50,  45 },  NO_EVO             },
    {  52, "Meowth",       "Normal",  nullptr,    { 40,  45,  35,  40,  40,  90 },  EVO(LEVEL, 28, 53) },
    {  53, "Persian",      "Normal",  nullptr,    { 65,  70,  60,  65,  65, 115 },  NO_EVO             },
    {  54, "Psyduck",      "Water",   nullptr,    { 50,  52,  48,  65,  50,  55 },  EVO(LEVEL, 33, 55) },
    {  55, "Golduck",      "Water",   nullptr,    { 80,  82,  78,  95,  80,  85 },  NO_EVO             },
    { 133, "Eevee",        "Normal",  nullptr,    { 55,  55,  50,  45,  65,  55 },  EVO(ITEM,  84,134) },
    { 134, "Vaporeon",     "Water",   nullptr,    {130,  65,  60, 110,  95,  65 },  NO_EVO             },
};

#undef NO_EVO
#undef EVO

inline constexpr std::size_t GEN1_SPECIES_COUNT =
    sizeof(GEN1_SPECIES) / sizeof(GEN1_SPECIES[0]);