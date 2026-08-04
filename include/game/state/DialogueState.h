#pragma once
#include <memory>
#include <string>
#include "IGameState.h"
#include "engine/input/KeyBindings.h"
#include "game/ui/MenuCommands.h"
#include "game/ui/DialogueBoxData.h"
#include "engine/services/GameServices.h"
#include "game/events/GameEvents.h"
#include <SFML/Graphics/Font.hpp>

class InputManager;

class DialogueState : public IGameState
{
public:
    DialogueState(GameServices<GameEvent> services,
                  std::string text, std::filesystem::path fontPath = {});
    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;
    bool BlocksRenderBelow() const override { return false; }

private:
    GameServices<GameEvent> services_;
    KeyBindings<MenuContext> navInput_;
    DialogueBoxData box_;
    std::string text_;
    std::filesystem::path fontPath_;
};