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

    src/game/world/World.cpp
    src/game/world/MapLoader.cpp
    src/game/world/Map.cpp

    src/engine/ecs/EntityFactory.cpp

    src/game/ecs/movement/MovementSystem.cpp
    src/game/gameplay/GameController.cpp
    src/game/gameplay/CameraSystem.cpp
    src/game/ecs/collision/CollisionSystem.cpp
    src/game/ecs/inventory/InventorySystem.cpp
    src/game/ecs/interaction/ItemPickupSystem.cpp

    src/engine/input/InputManager.cpp

    src/game/state/GameplayState.cpp
    src/game/state/PauseState.cpp
    src/game/state/InventoryState.cpp
    src/game/state/DialogueState.cpp
    src/game/state/StateFactory.cpp

    src/engine/render/RenderSystem.cpp
    src/engine/render/SFMLRenderer.cpp
    src/engine/render/atlases/Atlas.cpp
    src/engine/render/atlases/TileAtlas.cpp

    src/engine/assets/AssetDatabase.cpp
    src/engine/assets/metadata/AnimationAssetMetadata.cpp
    src/engine/assets/metadata/CollisionAssetMetadata.cpp
    src/engine/assets/metadata/RenderAssetMetadata.cpp
    src/engine/assets/metadata/AnimationAssetMetadata.cpp
    src/engine/assets/metadata/InteractionAssetMetadata.cpp
    src/engine/assets/metadata/ArchetypeDefinition.cpp
    src/engine/assets/metadata/ItemMetadata.cpp
    src/engine/assets/source/JsonMetadataSource.cpp

    src/engine/logging/Logger.cpp
    src/engine/logging/sinks/ConsoleSink.cpp
    src/engine/logging/sinks/FileSink.cpp

    src/engine/events/EventDispatcher.cpp

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