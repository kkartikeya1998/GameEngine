#pragma once
#include <vector>
#include "events/Events.h"

class EventQueue
{
public:
    void Push(GameEvent event) { pending_.push_back(std::move(event)); }

    // Called once per frame by Game, after states_.Update(dt).
    // Caller supplies the dispatch logic (std::visit lambda) so
    // EventQueue itself doesn't need to know about audio/UI/etc.
    template <typename Handler>
    void Drain(Handler&& handler)
    {
        auto batch = std::move(pending_);
        pending_.clear();
        for (auto& event : batch)
        {
            std::visit(handler, event);
        }
    }

private:
    std::vector<GameEvent> pending_;
};