#pragma once
#include <memory>
#include <string>
#include "IGameState.h"
#include "state/StateMachine.h"
#include "input/KeyBindings.h"
#include "ui/MenuCommands.h"
#include "ui/DialogueBoxData.h"
#include "game/GameServices.h"
#include <SFML/Graphics/Font.hpp>

class InputManager;

class DialogueState : public IGameState
{
public:
    DialogueState(GameServices services,
                  std::string text, std::filesystem::path fontPath = {});
    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;
    bool BlocksRenderBelow() const override { return false; }

private:
    GameServices services_;
    KeyBindings<MenuContext> navInput_;
    DialogueBoxData box_;
    std::string text_;
    std::filesystem::path fontPath_;
};