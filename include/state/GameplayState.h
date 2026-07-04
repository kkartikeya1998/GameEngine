#pragma once

#include <memory>
#include <string>
#include "IGameState.h"
#include "input/KeyBindings.h"
#include "tmp/movement/PlayerControlComponent.h"
#include "asset/AssetManager.h"
#include "system/GameController.h"

class InputManager;

class GameplayState : public IGameState {
public:
    GameplayState(InputManager& input, AssetManager& assets, const std::string& assetsRoot);

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem& renderSystem, float dt) override;

private:
    InputManager& input_;
    AssetManager& assets_;
    std::string assetsRoot_;

    KeyBindings<PlayerControlComponent> bindings_;
    std::unique_ptr<GameController> controller_;
};