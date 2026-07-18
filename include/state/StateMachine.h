#pragma once

#include <memory>
#include <vector>
#include <cassert>

#include "render/RenderSystem.h"

template <typename State>
class StateMachine
{
public:
    void Push(std::unique_ptr<State> state)
    {
        pending_.push_back(
            {Operation::Push, std::move(state)});
    }

    void Pop()
    {
        pending_.push_back(
            {Operation::Pop, nullptr});
    }

    void Replace(std::unique_ptr<State> state)
    {
        pending_.push_back(
            {Operation::Replace, std::move(state)});
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

    State *Top() const
    {
        return states_.empty() ? nullptr : states_.back().get();
    }

    void Render(RenderSystem &renderSystem, float dt)
    {
        std::vector<State *> toRender;

        for (auto it = states_.rbegin(); it != states_.rend(); ++it)
        {
            toRender.push_back(it->get());

            if ((*it)->BlocksRenderBelow())
                break;
        }

        // toRender is currently top-to-bottom; walk it bottom-to-top to find
        // the active camera (the state closest to the "world" owns it).
        Camera camera;
        for (auto it = toRender.rbegin(); it != toRender.rend(); ++it)
        {
            if (auto cam = (*it)->GetCamera())
            {
                camera = *cam;
                break;
            }
        }

        renderSystem.beginFrame(camera);

        for (auto it = toRender.rbegin(); it != toRender.rend(); ++it)
        {
            (*it)->Render(renderSystem, dt);
        }

        renderSystem.endFrame();
    }

    template <typename Getter>
    auto FindFirst(Getter &&getter) const -> decltype(getter(std::declval<State *>()))
    {
        for (auto it = states_.rbegin(); it != states_.rend(); ++it)
        {
            if (auto result = getter(it->get()))
                return result;
        }
        return decltype(getter(std::declval<State *>())){};
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
        std::unique_ptr<State> state;
    };

    void ApplyPendingOperations()
    {
        for (auto &op : pending_)
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

    std::vector<std::unique_ptr<State>> states_;
    std::vector<PendingOperation> pending_;
};