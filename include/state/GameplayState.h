#pragma once

#include <memory>
#include <string>
#include "game/GameServices.h"
#include "input/KeyBindings.h"
#include "component/PlayerControlComponent.h"
#include "system/GameController.h"
#include "render/Camera.h"
#include "state/IGameState.h"

class GameplayState : public IGameState
{
public:
    GameplayState(
        GameServices services,
        std::filesystem::path fontPath = {});

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;
    std::optional<Camera> GetCamera() const override { return camera_; }
    Registry *GetRegistry() const override { return controller_ ? &controller_->getWorld()->registry() : nullptr; }

private:
    GameServices services_;
    std::filesystem::path fontPath_;
    sf::Font font_;
    bool fontLoaded_ = false;

    KeyBindings<PlayerControlComponent> bindings_;
    std::unique_ptr<GameController> controller_;
    Camera camera_;
};