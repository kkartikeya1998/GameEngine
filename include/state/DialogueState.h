#pragma once
#include <memory>
#include <string>
#include "IGameState.h"
#include "state/StateMachine.h"
#include "input/KeyBindings.h"
#include "ui/MenuCommands.h"
#include "ui/DialogueBoxData.h"
#include "events/EventQueue.h"
#include <SFML/Graphics/Font.hpp>

class InputManager;

class DialogueState : public IGameState
{
public:
    DialogueState(InputManager &input, StateMachine<IGameState> &stateMachine, EventQueue &events,
                  std::string text, std::filesystem::path fontPath = {});
    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;
    bool BlocksRenderBelow() const override { return false; }

private:
    InputManager &input_;
    StateMachine<IGameState> &stateMachine_;
    EventQueue &events_;
    KeyBindings<MenuContext> navInput_;
    DialogueBoxData box_;
    std::string text_;
    std::filesystem::path fontPath_;
};