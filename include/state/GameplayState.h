#pragma once

#include <memory>
#include <string>
#include "IGameState.h"
#include "input/KeyBindings.h"
#include "component/PlayerControlComponent.h"
#include "asset/AssetDatabase.h"
#include "system/GameController.h"
#include "state/StateMachine.h"
#include "system/AnimationSystem.h"
#include "render/Camera.h"
#include "events/EventQueue.h"

class InputManager;

class GameplayState : public IGameState
{
public:
    GameplayState(InputManager &input, AssetDatabase &assets, StateMachine<IGameState> &stateMachine, AnimationSystem &animationSystem, EventQueue &events);

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;
    std::optional<Camera> GetCamera() const override { return camera_; }
    Registry *GetRegistry() const override { return controller_ ? &controller_->getWorld()->registry() : nullptr; }

private:
    InputManager &input_;
    AssetDatabase &assets_;
    StateMachine<IGameState> &stateMachine_;
    AnimationSystem &animationSystem_;
    EventQueue &events_;

    KeyBindings<PlayerControlComponent> bindings_;
    std::unique_ptr<GameController> controller_;
    Camera camera_;
};