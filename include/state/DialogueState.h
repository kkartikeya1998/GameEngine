#pragma once
#include <memory>
#include <string>
#include "IGameState.h"
#include "state/StateMachine.h"
#include "input/KeyBindings.h"
#include "ui/MenuContext.h"
#include "ui/DialogueBox.h"
#include "events/EventQueue.h"
#include <SFML/Graphics/Font.hpp>

class InputManager;

class DialogueState : public IGameState {
public:
    DialogueState(InputManager& input, StateMachine<IGameState>& stateMachine, EventQueue& events, std::string text);
    void Update(float dt) override;
    void Render(RenderSystem& renderSystem, float dt) override;
    bool BlocksRenderBelow() const override { return false; }

private:
    InputManager& input_;
    StateMachine<IGameState>& stateMachine_;
    EventQueue& events_;
    KeyBindings<MenuContext> navInput_;
    std::unique_ptr<DialogueBox> box_;
    std::string text_;
    static sf::Font s_font_;
};