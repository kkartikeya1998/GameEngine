cmake_minimum_required(VERSION 3.20)

project(PokemonProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PokemonProject
    src/main.cpp
    src/world/Map.cpp
    src/world/World.cpp
    
    src/entities/pokemon/Pokemon.cpp
    src/entities/pokemon/Stats.cpp
    src/entities/pokemon/WildPokemon.cpp
    src/entities/pokemon/WildPokemonFactory.cpp
    src/entities/pokemon/CapturedPokemon.cpp
    src/entities/pokemon/CapturedPokemonFactory.cpp
    src/entities/pokemon/PokemonSpeciesRepository.cpp
)

target_include_directories(PokemonProject PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/assets
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})