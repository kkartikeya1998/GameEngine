#pragma once
#include "input/ICommand.h"

// Semantic navigation intent, decoupled from raw keys — mirrors gameplay's Direction pattern
struct MenuContext {
    bool navigateUp = false;
    bool navigateDown = false;
    bool confirm = false;
    bool cancel = false;
};

// Sets navigateUp when bound key is triggered
class NavigateUpCommand : public ICommand<MenuContext> {
public:
    void execute(MenuContext &ctx) const override { ctx.navigateUp = true; }
};

// Sets navigateDown when bound key is triggered
class NavigateDownCommand : public ICommand<MenuContext> {
public:
    void execute(MenuContext &ctx) const override { ctx.navigateDown = true; }
};

// Sets confirm when bound key is triggered
class ConfirmCommand : public ICommand<MenuContext> {
public:
    void execute(MenuContext &ctx) const override { ctx.confirm = true; }
};

// Sets cancel when bound key is triggered
class CancelCommand : public ICommand<MenuContext> {
public:
    void execute(MenuContext &ctx) const override { ctx.cancel = true; }
};