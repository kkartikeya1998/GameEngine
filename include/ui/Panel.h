#pragma once
#include <memory>
#include <string>
#include <vector>
#include "input/ICommand.h"
#include "ui/UIType.h"

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
    float x = 0.f;
    float y = 0.f;
    float width = 0.f;
    float height = 0.f;
};