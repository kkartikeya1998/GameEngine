#pragma once
#include "IGameState.h"
#include "state/StateMachine.h"
#include "input/KeyBindings.h"
#include "ui/MenuCommands.h"
#include "ui/Panel.h"
#include "entities/Registry.h"
#include "component/InventoryComponent.h"
#include "events/EventQueue.h"
#include "asset/AssetDatabase.h"

#include <string>

class InputManager;

struct InventoryActionContext
{
    InventoryComponent *inventory;
    const AssetDatabase &assets;
    EntityID player;
    StateMachine<IGameState> &stateMachine;
    EventQueue &events;
};

class InventoryState : public IGameState
{
public:
    InventoryState(InputManager &input,
                   StateMachine<IGameState> &stateMachine,
                   Registry &registry,
                   EntityID player,
                   EventQueue &events,
                   AssetDatabase &assets,
                   bool *openFlag = nullptr,
                   std::filesystem::path fontPath = {}); // override per state if ever needed

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;

private:
    void RefreshOptions();

    InputManager &input_;
    StateMachine<IGameState> &stateMachine_;
    Registry &registry_;
    EntityID player_;
    EventQueue &events_;
    AssetDatabase &assets_;

    bool *openFlag_;

    KeyBindings<MenuContext> navInput_;
    Panel<InventoryActionContext> panel_;

    std::filesystem::path fontPath_; // resolved through UIFont::GetShared at render time
};