#include <iostream>

#include "state/PauseState.h"
#include "utils/StateMachine.h"
#include "input/InputManager.h"

PauseState::PauseState(InputManager& input, StateMachine<IGameState>& stateMachine)
    : input_(input), stateMachine_(stateMachine)
{
    // std::cout << "[PauseState] Created\n";
}

void PauseState::Update(float dt)
{
    // std::cout << "[PauseState] Update...\n";
    if (input_.WasKeyPressed(Key::Escape)) {
        stateMachine_.Pop(); // back to GameplayState — OnResume() fires there, controller_ untouched
    }
}

void PauseState::Render(RenderSystem& renderSystem, float dt)
{
    renderSystem.submitRect(
        RenderLayer::ScreenOverlay, 0.f,
        0.f, 0.f,
        GameConstants::GAME_RESOLUTION_W, GameConstants::GAME_RESOLUTION_H,
        sf::Color(0, 0, 0, 150),
        /*screenSpace=*/true);
}