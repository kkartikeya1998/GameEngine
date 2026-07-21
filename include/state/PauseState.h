#pragma once

#include "IGameState.h"
#include "state/StateMachine.h"
#include "input/KeyBindings.h"
#include "ui/MenuCommands.h"
#include "ui/Panel.h"

#include <SFML/Graphics/Font.hpp>

class InputManager;

// Context passed to pause menu action commands — only what Resume/Quit need
struct PauseActionContext
{
    StateMachine<IGameState> &stateMachine;
    InputManager &input;
};

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
               StateMachine<IGameState> &stateMachine,
               std::filesystem::path fontPath = {});

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;

private:
    InputManager &input_;
    StateMachine<IGameState> &stateMachine_;

    KeyBindings<MenuContext> navInput_; // Up/Down/Enter/Escape -> menu intent
    Panel<PauseActionContext> panel_;   // Continue / Quit options

    std::filesystem::path fontPath_;
};