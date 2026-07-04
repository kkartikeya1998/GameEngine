#pragma once

#include "IGameState.h"
#include "utils/StateMachine.h"

class InputManager;
// ---------------------------------------------------------------------------
// PauseState — sits on top of GameplayState. Doesn't touch
// GameController at all; BlocksUpdateBelow (default true) freezes
// gameplay logic, BlocksRenderBelow=false lets the frozen world still
// render behind whatever pause UI this draws.
// ---------------------------------------------------------------------------
class PauseState : public IGameState {
public:
    PauseState(InputManager& input, StateMachine<IGameState>& stateMachine);

    void Update(float dt) override;
    void Render(RenderSystem& renderSystem, float dt) override;

    bool BlocksRenderBelow() const override { return false; }

private:
    InputManager& input_;
    StateMachine<IGameState>& stateMachine_;
};