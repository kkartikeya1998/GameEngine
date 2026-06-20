#pragma once

#include <memory>
#include <SFML/System/Clock.hpp>

#include "system/GameController.h"
#include "render/RenderSystem.h"

// ---------------------------------------------------------------------------
// Game — top-level owner of the game loop.
//
// Holds GameController (world + player state) and RenderSystem (drawing).
// All texture/JSON wiring lives inside SFMLRenderer now — Game only knows
// file paths, not atlases or repositories.
// ---------------------------------------------------------------------------
class Game {
public:
    Game();

    void run();

private:
    GameController controller_;
    std::unique_ptr<RenderSystem> renderSystem_;

    // Owned here (not global) so multiple Game instances, if ever needed,
    // don't share one clock.
    sf::Clock gameClock_;
};