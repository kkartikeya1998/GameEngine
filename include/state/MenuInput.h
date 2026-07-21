#pragma once
#include "input/KeyBindings.h"
#include "ui/MenuCommands.h"

// Shared key-binding recipes for menu-driven states — keeps Up/Down/Select/Back
// wiring in one place instead of copy-pasted into every state constructor.
namespace MenuInput
{
    // Binds navigation (Up/Down) and one Select key. Call BindBackKey separately —
    // states differ on how many/which back keys they want (Pause: Escape only;
    // Inventory: Escape + I), so it's not folded into this one call.
    inline void BindDefaults(KeyBindings<MenuContext> &bindings, Key selectKey = Key::Enter, Key escapeKey = Key::Escape)
    {
        bindings.bind(Key::Up, std::make_shared<NavigateUpCommand>(), TriggerMode::Press);
        bindings.bind(Key::Down, std::make_shared<NavigateDownCommand>(), TriggerMode::Press);
        bindings.bind(selectKey, std::make_shared<ConfirmCommand>(), TriggerMode::Press);
        bindings.bind(escapeKey, std::make_shared<CancelCommand>(), TriggerMode::Press);
    }

    inline void BindBackKey(KeyBindings<MenuContext> &bindings, Key backKey)
    {
        bindings.bind(backKey, std::make_shared<CancelCommand>(), TriggerMode::Press);
    }
}