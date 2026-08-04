#pragma once

#include <memory>
#include <string>
#include "engine/services/GameServices.h"
#include "engine/input/KeyBindings.h"
#include "game/ecs/player/PlayerControlComponent.h"
#include "game/ecs/animation/AnimationSystem.h"
#include "game/gameplay/GameController.h"
#include "game/events/GameplayEventHandler.h"
#include "game/ecs/interaction/InteractionManager.h"
#include "engine/render/Camera.h"
#include "game/state/IGameState.h"

class GameplayState : public IGameState
{
public:
    explicit GameplayState(
        GameServices<GameEvent> services,
        std::filesystem::path fontPath = Assets::Fonts::PIXFAY);

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;
    std::optional<Camera> GetCamera() const override { return camera_; }
    Registry *GetRegistry() const override { return controller_ ? &controller_->getWorld()->registry() : nullptr; }

private:
    GameServices<GameEvent> services_;
    InteractionManager interactions_;
    AnimationSystem animationSystem_;
    std::unique_ptr<GameplayEventHandler> eventHandler_;
    std::filesystem::path fontPath_;
    sf::Font font_;
    bool fontLoaded_ = false;

    KeyBindings<PlayerControlComponent> bindings_;
    std::unique_ptr<GameController> controller_;
    Camera camera_;
};