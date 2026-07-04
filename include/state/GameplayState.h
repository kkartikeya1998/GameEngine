#pragma once

#include <memory>
#include <string>
#include "IGameState.h"
#include "input/KeyBindings.h"
#include "tmp/movement/PlayerControlComponent.h"
#include "asset/AssetManager.h"
#include "system/GameController.h"
#include "utils/StateMachine.h"

class InputManager;

class GameplayState : public IGameState {
public:
    GameplayState(InputManager& input, AssetManager& assets, StateMachine<IGameState>& stateMachine);

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem& renderSystem, float dt) override;

private:
    InputManager& input_;
    AssetManager& assets_;
    StateMachine<IGameState>& stateMachine_;

    KeyBindings<PlayerControlComponent> bindings_;
    std::unique_ptr<GameController> controller_;
};