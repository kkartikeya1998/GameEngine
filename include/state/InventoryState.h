#pragma once
#include "IGameState.h"
#include "state/StateMachine.h"
#include "input/KeyBindings.h"
#include "ui/MenuCommands.h"
#include "ui/Panel.h"
#include "entities/Registry.h"
#include "component/InventoryComponent.h"
#include "game/GameServices.h"

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
    InventoryState(GameServices services,
                   Registry &registry,
                   EntityID player,
                   bool *openFlag = nullptr,
                   std::filesystem::path fontPath = {});

    void OnEnter() override;
    void OnExit() override;
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;

private:
    void RefreshOptions();

    GameServices services_;
    Registry &registry_;
    EntityID player_;

    bool *openFlag_;
    bool needsRefresh_ = false;

    KeyBindings<MenuContext> navInput_;
    Panel<InventoryActionContext> panel_;

    std::filesystem::path fontPath_;
};