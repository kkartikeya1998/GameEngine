#pragma once

#include <memory>
#include <string>
#include "IGameState.h"
#include "input/KeyBindings.h"
#include "tmp/component/PlayerControlComponent.h"
#include "asset/AssetManager.h"
#include "system/GameController.h"
#include "utils/StateMachine.h"
#include "system/AnimationSystem.h"
#include "render/Camera.h"

class InputManager;

class GameplayState : public IGameState {
public:
    GameplayState(InputManager& input, AssetManager& assets, StateMachine<IGameState>& stateMachine, AnimationSystem& animationSystem);

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem& renderSystem, float dt) override;
    std::optional<Camera> GetCamera() const override { return camera_; }

private:
    InputManager& input_;
    AssetManager& assets_;
    StateMachine<IGameState>& stateMachine_;
    AnimationSystem& animationSystem_;

    KeyBindings<PlayerControlComponent> bindings_;
    std::unique_ptr<GameController> controller_;
    Camera camera_;
};