#include <iostream>

#include "state/PauseState.h"
#include "state/StateMachine.h"
#include "input/InputManager.h"
#include "asset/AsssetPaths.h"

sf::Font PauseState::s_font_;

PauseState::PauseState(InputManager &input,
                       StateMachine<IGameState> &stateMachine)
    : input_(input), stateMachine_(stateMachine), font_(s_font_)
{
    std::cout << "[PauseState] Created\n";

    // Lazy initialization: load once on first construction
    static bool s_loaded = false;
    if (!s_loaded)
    {
        if (!s_font_.openFromFile(Assets::Fonts::PIXFAY))
        {
            std::cerr << "[PauseState] Failed to load font: "
                      << Assets::Fonts::PIXFAY << "\n";
        }
        else
        {
            s_loaded = true;
            std::cout << "[PauseState] Font loaded: "
                      << Assets::Fonts::PIXFAY << "\n";
        }
    }
}

void PauseState::Update(float dt)
{

    std::cout << "[PauseState] Starting Update...\n";
    if (input_.WasKeyPressed(Key::Escape))
    {
        input_.RequestQuit();
    }
    if (input_.WasKeyPressed(Key::Enter))
    {
        stateMachine_.Pop(); // back to GameplayState — OnResume() fires there, controller_ untouched
    }

    std::cout << "[PauseState] Ending Update...\n";
}

void PauseState::Render(RenderSystem &renderSystem, float dt)
{
    std::cout << "[PauseState] Starting Render...\n";
    // 1. Fullscreen translucent black overlay (as you already had).
    renderSystem.submitRect(
        RenderLayer::ScreenOverlay, 0.f,
        0.f, 0.f,
        GameConstants::GAME_RESOLUTION_W,
        GameConstants::GAME_RESOLUTION_H,
        sf::Color(0, 0, 0, 150),
        /*screenSpace=*/true);

    std::cout << "[PauseState] Submitted first rect.\n";
    // 2. Dialogue box rectangle in screen space.
    const float boxWidth = 600.f;
    const float boxHeight = 120.f;

    const float boxX = (GameConstants::GAME_RESOLUTION_W - boxWidth) * 0.5f;
    const float boxY = (GameConstants::GAME_RESOLUTION_H - boxHeight) * 0.7f;

    std::cout << "[PauseState] Submitting second rect.\n";
    renderSystem.submitRect(
        RenderLayer::ScreenOverlay, 1.f, // draw above tint
        boxX, boxY,
        boxWidth, boxHeight,
        sf::Color(0, 0, 0, 200), // slightly darker panel
        /*screenSpace=*/true);

    std::cout << "[PauseState] Submitted second rect.\n";
    // 3. Dialogue text inside the box.
    const std::string message =
        "Press ESC to quit\n"
        "Press ENTER to continue";

    const float paddingX = 20.f;
    const float paddingY = 20.f;

    std::cout << "[PauseState] Submitting text.\n";
    renderSystem.submitText(
        RenderLayer::ScreenOverlay, 2.f, // on top of box
        font_,                           // PauseState member: const sf::Font& font_;
        message,
        boxX + paddingX,
        boxY + paddingY,
        24u,
        sf::Color::White,
        /*screenSpace=*/true);

    std::cout << "[PauseState] Ending Render...\n";
}