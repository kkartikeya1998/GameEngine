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
    // std::cout << "[PauseState] Rendering overlay (not implemented yet)\n";
    // TODO: draw a pause overlay/menu
}