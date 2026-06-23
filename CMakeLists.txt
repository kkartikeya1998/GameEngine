cmake_minimum_required(VERSION 3.20)

project(PokemonProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch SFML 3.1 from GitHub
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.1.0
    GIT_SHALLOW    ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)

add_executable(PokemonProject
    src/main.cpp

    src/world/Map.cpp
    src/world/World.cpp
    src/world/MapLoader.cpp
    src/world/MapObject.cpp
    src/world/MapObjectRepository.cpp
    src/world/MapRepository.cpp
    src/world/SpriteRepository.cpp
    

    src/entities/pokemon/Pokemon.cpp
    src/entities/pokemon/Stats.cpp
    src/entities/pokemon/WildPokemon.cpp
    src/entities/pokemon/WildPokemonFactory.cpp
    src/entities/pokemon/CapturedPokemon.cpp
    src/entities/pokemon/CapturedPokemonFactory.cpp
    src/entities/pokemon/PokemonSpeciesRepository.cpp
    
    src/entities/Entity.cpp
    src/entities/player/Player.cpp
    src/entities/movement/GridMovementMechanics.cpp
    src/entities/animation/AnimationComponent.cpp

    src/system/GameController.cpp
    
    src/render/RenderSystem.cpp
    src/render/SFMLRenderer.cpp
    src/render/Atlas.cpp
    src/render/TileAtlas.cpp
    src/render/SpriteAtlas.cpp
    src/render/MapObjectAtlas.cpp

    src/Game.cpp
    )

target_include_directories(PokemonProject PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/assets
)

target_compile_definitions(PokemonProject PRIVATE
    PROJECT_ROOT=\"${CMAKE_SOURCE_DIR}\"
)

target_link_libraries(PokemonProject PRIVATE SFML::Graphics SFML::Window SFML::System)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_BINARY_DIR})