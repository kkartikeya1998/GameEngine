#pragma once

#include <memory>
#include <string>
#include <SFML/System/Clock.hpp>

#include "asset/AssetManager.h"
#include "system/GameController.h"
#include "render/RenderSystem.h"

// ---------------------------------------------------------------------------
// Game — top-level owner of the game loop.
//
// CHANGED: Game no longer owns TileRepository, MapObjectRepository, and
// SpriteRepository as three separate members. It owns one AssetManager,
// which loads and owns all three internally (see AssetManager.cpp for the
// full list of what gets loaded at startup). Game pulls individual
// repository references back out via assets_.get<T>() wherever a
// downstream class (GameController, SFMLRenderer) still expects a
// concrete repository reference — those classes don't know AssetManager
// exists, only Game does.
//
// assetsRoot_ is the single source of truth for where assets live on
// disk. Previously this path was hardcoded independently in two places
// (Game's old member-init list, and GameController.cpp's own
// PROJECT_ROOT-based path for World) — now there's exactly one root,
// passed into both AssetManager and GameController.
//
// Declaration order matters: assetsRoot_ must be set before assets_ is
// constructed; assets_ must be constructed before GameController (which
// needs a loaded MapObjectRepository&) and before renderSystem_ (whose
// SFMLRenderer takes references to all three repositories).
// ---------------------------------------------------------------------------
class Game {
public:
    Game();

    void run();

private:
    // Single source of truth for the asset tree's location on disk.
    std::string assetsRoot_;

    // Owns and loads every metadata repository. Constructed second, right
    // after assetsRoot_ is known.
    AssetManager assets_;

    GameController controller_;
    std::unique_ptr<RenderSystem> renderSystem_;

    sf::Clock gameClock_;
};