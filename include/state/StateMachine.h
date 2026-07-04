#pragma once

#include <memory>
#include <vector>
#include <cassert>

#include "IGameState.h"

class StateMachine {
public:
    void Push(std::unique_ptr<IGameState> state)
    {
        pending_.push_back(
            { Operation::Push, std::move(state) });
    }

    void Pop()
    {
        pending_.push_back(
            { Operation::Pop, nullptr });
    }

    void Replace(std::unique_ptr<IGameState> state)
    {
        pending_.push_back(
            { Operation::Replace, std::move(state) });
    }

    void Update(float dt)
    {
        for (auto it = states_.rbegin(); it != states_.rend(); ++it)
        {
            (*it)->Update(dt);

            if ((*it)->BlocksUpdateBelow())
                break;
        }

        ApplyPendingOperations();
    }

    void Render(RenderSystem& renderSystem, float dt)
    {
        std::vector<IGameState*> toRender;

        for (auto it = states_.rbegin(); it != states_.rend(); ++it)
        {
            toRender.push_back(it->get());

            if ((*it)->BlocksRenderBelow())
                break;
        }

        for (auto it = toRender.rbegin(); it != toRender.rend(); ++it)
        {
            (*it)->Render(renderSystem, dt);
        }
    }

    bool Empty() const
    {
        return states_.empty();
    }

private:

    enum class Operation
    {
        Push,
        Pop,
        Replace
    };

    struct PendingOperation
    {
        Operation operation;
        std::unique_ptr<IGameState> state;
    };

    void ApplyPendingOperations()
    {
        for (auto& op : pending_)
        {
            switch (op.operation)
            {
            case Operation::Push:
            {
                states_.push_back(std::move(op.state));
                states_.back()->OnEnter();
                break;
            }

            case Operation::Pop:
            {
                if (!states_.empty())
                {
                    states_.back()->OnExit();
                    states_.pop_back();
                }
                break;
            }

            case Operation::Replace:
            {
                if (!states_.empty())
                {
                    states_.back()->OnExit();
                    states_.pop_back();
                }

                states_.push_back(std::move(op.state));
                states_.back()->OnEnter();
                break;
            }
            }
        }

        pending_.clear();
    }

    std::vector<std::unique_ptr<IGameState>> states_;
    std::vector<PendingOperation> pending_;
};