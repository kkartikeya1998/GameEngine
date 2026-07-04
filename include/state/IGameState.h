#pragma once

class InputManager;
class RenderSystem;

class IGameState {
public:
    virtual ~IGameState() = default;
    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual void Update(float dt) = 0;
    virtual void Render(RenderSystem& renderSystem, float dt) = 0;

    virtual bool BlocksUpdateBelow() const { return true; }
    virtual bool BlocksRenderBelow() const { return false; }
};