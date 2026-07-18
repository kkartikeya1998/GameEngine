cmake_minimum_required(VERSION 3.20)

project(PokemonProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug if the user didn't specify one 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

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

    src/world/World.cpp
    src/world/MapLoader.cpp
    src/world/Map.cpp

    src/entities/EntityFactory.cpp

    src/system/MovementSystem.cpp
    src/system/GameController.cpp
    src/system/CameraSystem.cpp
    src/system/CollisionSystem.cpp
    src/system/InventorySystem.cpp
    src/system/ItemPickupSystem.cpp

    src/input/InputManager.cpp

    src/state/GameplayState.cpp
    src/state/PauseState.cpp
    src/state/InventoryState.cpp
    src/state/DialogueState.cpp

    src/render/RenderSystem.cpp
    src/render/SFMLRenderer.cpp
    src/render/atlases/Atlas.cpp
    src/render/atlases/TileAtlas.cpp

    src/asset/AssetDatabase.cpp
    src/asset/metadata/AnimationAssetMetadata.cpp
    src/asset/metadata/CollisionAssetMetadata.cpp
    src/asset/metadata/RenderAssetMetadata.cpp
    src/asset/metadata/AnimationAssetMetadata.cpp
    src/asset/metadata/InteractionAssetMetadata.cpp
    src/asset/metadata/ArchetypeDefinition.cpp
    src/asset/metadata/ItemMetadata.cpp
    src/asset/source/JsonMetadataSource.cpp

    src/log/Logger.cpp
    src/log/sinks/ConsoleSink.cpp
    src/log/sinks/FileSink.cpp

    src/Game.cpp
    )

target_include_directories(PokemonProject PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/assets
)

target_compile_definitions(PokemonProject PRIVATE
    PROJECT_ROOT=\"${CMAKE_SOURCE_DIR}\"
    $<$<CONFIG:Debug>:ENGINE_DEBUG>
)

target_link_libraries(PokemonProject PRIVATE SFML::Graphics SFML::Window SFML::System)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_BINARY_DIR})