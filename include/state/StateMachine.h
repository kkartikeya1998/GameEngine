#pragma once

#include <memory>
#include <vector>
#include "IGameState.h"

class StateMachine {
public:
    void Push(std::unique_ptr<IGameState> state)
    {
        if (!states_.empty()) states_.back()->OnExit();
        states_.push_back(std::move(state));
        states_.back()->OnEnter();
    }

    void Pop()
    {
        if (states_.empty()) return;
        states_.back()->OnExit();
        states_.pop_back();
        if (!states_.empty()) states_.back()->OnEnter();
    }

    void Replace(std::unique_ptr<IGameState> state)
    {
        Pop();
        Push(std::move(state));
    }

    void Update(float dt)
    {
        for (auto it = states_.rbegin(); it != states_.rend(); ++it) {
            (*it)->Update(dt);
            if ((*it)->BlocksUpdateBelow()) break;
        }
    }

    void Render(RenderSystem& renderSystem, float dt)
    {
        std::vector<IGameState*> toRender;
        for (auto it = states_.rbegin(); it != states_.rend(); ++it) {
            toRender.push_back(it->get());
            if ((*it)->BlocksRenderBelow()) break;
        }
        for (auto it = toRender.rbegin(); it != toRender.rend(); ++it)
            (*it)->Render(renderSystem, dt);
    }

private:
    std::vector<std::unique_ptr<IGameState>> states_;
};