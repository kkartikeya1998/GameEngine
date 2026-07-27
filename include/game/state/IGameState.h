#pragma once

#include <optional>
#include "engine/render/Camera.h"
#include "engine/input/InputManager.h"
#include "engine/render/RenderSystem.h"
#include "engine/ecs/Registry.h"

class IGameState
{
public:
    virtual ~IGameState() = default;
    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual void Update(float dt) = 0;
    virtual void Render(RenderSystem &renderSystem, float dt) = 0;

    virtual bool BlocksUpdateBelow() const { return true; }
    virtual bool BlocksRenderBelow() const { return false; }

    virtual Registry* GetRegistry() const { return nullptr; }

    // Most states (pause menus, overlays) don't own a camera and
    // inherit whatever the state below them established.
    virtual std::optional<Camera> GetCamera() const { return std::nullopt; }
};