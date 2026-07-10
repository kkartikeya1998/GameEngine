#include <iostream>
#include "state/InventoryState.h"
#include "input/InputManager.h"
#include "asset/AsssetPaths.h"
#include "ui/UISystem.h"
#include "state/MenuInput.h"

sf::Font InventoryState::s_font_;

class UseItemCommand : public ICommand<InventoryActionContext>
{
public:
    explicit UseItemCommand(int itemIndex) : itemIndex_(itemIndex) {}
    void execute(InventoryActionContext &ctx) const override
    {
        if (!ctx.inventory)
            return;
        auto &items = ctx.inventory->items;
        if (itemIndex_ < 0 || itemIndex_ >= static_cast<int>(items.size()))
            return;

        items[itemIndex_].quantity -= 1;
        if (items[itemIndex_].quantity <= 0)
            items.erase(items.begin() + itemIndex_);

        ctx.events.Push(ItemConsumed{});
    }

private:
    int itemIndex_;
};

class CloseInventoryCommand : public ICommand<InventoryActionContext>
{
public:
    void execute(InventoryActionContext &ctx) const override
    {
        ctx.stateMachine.Pop();
        ctx.events.Push(ItemConsumed{}); // [TODO] : Remove. For testing purposes. Should show on selecting close, but not pressing I again
    }
};

InventoryState::InventoryState(InputManager &input,
                               StateMachine<IGameState> &stateMachine,
                               Entity &player,
                               EventQueue &events,
                               bool *openFlag)
    : input_(input), stateMachine_(stateMachine), player_(player), events_(events), openFlag_(openFlag), font_(s_font_)
{
    static bool s_loaded = false;
    if (!s_loaded)
    {
        if (!s_font_.openFromFile(Assets::Fonts::PIXFAY))
            std::cerr << "[InventoryState] Failed to load font\n";
        else
            s_loaded = true;
    }

    MenuInput::BindDefaults(navInput_); // Up/Down/Enter
    MenuInput::BindBackKey(navInput_, Key::Escape);
    MenuInput::BindBackKey(navInput_, Key::I); // toggle-close

    panel_.type = UIType::NonDiegetic;
    panel_.title = "Inventory";
    panel_.x = 200.f;
    panel_.y = 100.f;
    panel_.width = 400.f;
    panel_.height = 300.f;
}

void InventoryState::OnEnter()
{
    RefreshOptions();
}

void InventoryState::OnExit()
{
    if (openFlag_)
        *openFlag_ = false;
}

void InventoryState::RefreshOptions()
{
    panel_.options.clear();

    if (auto *inv = player_.get<InventoryComponent>())
    {
        for (int i = 0; i < static_cast<int>(inv->items.size()); ++i)
        {
            const auto &stack = inv->items[i];
            panel_.options.push_back(
                {stack.displayName + " x" + std::to_string(stack.quantity),
                 std::make_shared<UseItemCommand>(i)});
        }
    }

    panel_.options.push_back({"Close", std::make_shared<CloseInventoryCommand>()});

    if (panel_.selectedIndex >= static_cast<int>(panel_.options.size()))
        panel_.selectedIndex = 0;
}

void InventoryState::Update(float dt)
{
    MenuContext nav = navInput_.poll(input_);
    if (UISystem::HandleDefaultBack(nav, stateMachine_))
        return; // OnExit() fires here, clearing openFlag_

    InventoryActionContext actionCtx{player_.get<InventoryComponent>(), stateMachine_, events_};
    UISystem::HandleNavigation(panel_, nav, actionCtx);

    if (nav.confirm)
        RefreshOptions();
}

void InventoryState::Render(RenderSystem &renderSystem, float dt)
{
    UISystem::Render(panel_, renderSystem, font_);
}