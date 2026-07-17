#include <iostream>
#include "state/InventoryState.h"
#include "input/InputManager.h"
#include "asset/AsssetPaths.h"
#include "ui/UISystem.h"
#include "state/MenuInput.h"

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
                               Registry &registry,
                               EntityID player,
                               EventQueue &events,
                               bool *openFlag,
                               std::filesystem::path fontPath)
    : input_(input), stateMachine_(stateMachine), registry_(registry), player_(player),
      events_(events), openFlag_(openFlag)
{

    fontPath_ = fontPath.empty() ? std::filesystem::path(Assets::Fonts::PIXFAY) : std::move(fontPath);
    MenuInput::BindDefaults(navInput_);
    MenuInput::BindBackKey(navInput_, Key::Escape);
    MenuInput::BindBackKey(navInput_, Key::I);

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

    if (auto *inv = registry_.get<InventoryComponent>(player_))
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

    InventoryActionContext actionCtx{registry_.get<InventoryComponent>(player_), stateMachine_, events_};
    UISystem::HandleNavigation(panel_, nav, actionCtx);

    if (nav.confirm)
        RefreshOptions();
}

void InventoryState::Render(RenderSystem &renderSystem, float dt)
{
    UISystem::Render(panel_, renderSystem, UIFont::GetShared(fontPath_));
}