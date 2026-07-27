#pragma once

#include <memory>
#include <vector>
#include "InputManager.h"
#include "ICommand.h"

enum class TriggerMode { Hold, Press };

// Owned by a single state. Rebinding controls = calling bind() again,
// e.g. loading a saved keymap profile — the state's Update() logic
// never changes.
template <typename Context>
class KeyBindings {
public:
    void bind(Key key, std::shared_ptr<ICommand<Context>> command, TriggerMode mode = TriggerMode::Hold)
    {
        for (auto& b : bindings_) {
            if (b.key == key) { b.command = std::move(command); b.mode = mode; return; }
        }
        bindings_.push_back({key, std::move(command), mode});
    }

    Context poll(const InputManager& input) const
    {
        Context ctx{};
        for (const auto& b : bindings_) {
            bool trigger = (b.mode == TriggerMode::Hold) ? input.IsKeyDown(b.key) : input.WasKeyPressed(b.key);
            if (trigger)
                b.command->execute(ctx);
        }
        return ctx;
    }

private:
    struct Binding {
        Key key;
        std::shared_ptr<ICommand<Context>> command;
        TriggerMode mode;
    };
    std::vector<Binding> bindings_;
    // Bindings execute in insertion order; for fields like `direction`
    // that a later command can overwrite, the last matching bound key
    // wins if two opposing keys are held simultaneously — a behavior
    // change from the old if/else-if chain worth knowing about.
};