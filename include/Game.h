#pragma once

#include <memory>
#include <SFML/System/Clock.hpp>

#include "system/GameController.h"
#include "render/RenderSystem.h"
#include "world/MapRepository.h"
#include "world/MapObjectRepository.h"
#include "world/SpriteRepository.h"

// ---------------------------------------------------------------------------
// Game — top-level owner of the game loop.
//
// Game is now the single owner of every metadata repository —
// MapRepository, MapObjectRepository, and SpriteRepository. Previously,
// World and SFMLRenderer each owned their own private copies, meaning the
// same JSON files (object_metadata.json, tile metadata, sprite metadata)
// were parsed twice into two unrelated objects. Now there is exactly one
// instance of each, and both World (via MapLoader) and SFMLRenderer (via
// its atlases) hold references into Game's copies.
//
// Declaration order matters: repositories must be constructed before
// GameController (which builds World, which builds MapLoader with a
// reference to objectRepo_) and before renderSystem_ (whose SFMLRenderer
// takes references to all three repositories).
// ---------------------------------------------------------------------------
class Game {
public:
    Game();

    void run();

private:
    // Single shared metadata, constructed first.
    MapRepository tileRepo_;
    MapObjectRepository objectRepo_;
    SpriteRepository spriteRepo_;

    GameController controller_;
    std::unique_ptr<RenderSystem> renderSystem_;

    sf::Clock gameClock_;
};