#pragma once

#include "IGameState.h"
#include "game/state/StateMachine.h"
#include "engine/input/KeyBindings.h"
#include "game/ui/MenuCommands.h"
#include "game/ui/Panel.h"
#include "engine/services/GameServices.h"
#include "game/events/GameEvents.h" // TODO: replace by PauseEventHandler
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
    PauseState(GameServices<GameEvent> services,
               std::filesystem::path fontPath = {});

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;

private:
    GameServices<GameEvent> services_;

    KeyBindings<MenuContext> navInput_;
    Panel<PauseActionContext> panel_;

    std::filesystem::path fontPath_;
};