#pragma once
#include <memory>
#include <string>
#include <vector>
#include "engine/input/ICommand.h"
#include "game/ui/UIType.h"
#include "game/ui/UILayout.h"

// One selectable entry in a Panel — label plus the action it triggers on confirm
template <typename ActionContext>
struct MenuOption {
    std::string label;
    std::shared_ptr<ICommand<ActionContext>> command;
};

// Pure-data menu: a titled list of options with a cursor — logic lives in UISystem
template <typename ActionContext>
struct Panel {
    UIType type = UIType::NonDiegetic;
    std::string title;
    std::vector<MenuOption<ActionContext>> options;
    int selectedIndex = 0;
    UILayout layout; // resolved to a UIRect at render time by UILayoutResolver
};