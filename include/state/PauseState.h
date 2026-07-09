#pragma once

#include "IGameState.h"
#include "state/StateMachine.h"

#include <SFML/Graphics/Font.hpp>

class InputManager;
// ---------------------------------------------------------------------------
// PauseState — sits on top of GameplayState. Doesn't touch
// GameController at all; BlocksUpdateBelow (default true) freezes
// gameplay logic, BlocksRenderBelow=false lets the frozen world still
// render behind whatever pause UI this draws.
// ---------------------------------------------------------------------------

class PauseState : public IGameState
{
public:
    PauseState(InputManager &input,
               StateMachine<IGameState> &stateMachine);

    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;

private:
    InputManager &input_;
    StateMachine<IGameState> &stateMachine_;

    static sf::Font s_font_;
    const sf::Font &font_; // refers to s_font_
};