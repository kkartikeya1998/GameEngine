#pragma once
#include "IGameState.h"
#include "state/StateMachine.h"
#include "input/KeyBindings.h"
#include "ui/MenuContext.h"
#include "ui/Panel.h"
#include "entities/Entity.h"
#include "component/InventoryComponent.h"
#include "events/EventQueue.h"

#include <SFML/Graphics/Font.hpp>

class InputManager;

// Context passed to inventory action commands — the component they mutate, plus a way to close
struct InventoryActionContext {
    InventoryComponent *inventory; // nullable — Close doesn't need it, UseItemCommand checks before use
    StateMachine<IGameState> &stateMachine;
   EventQueue &events;
};
class InventoryState : public IGameState
{
public:
    InventoryState(InputManager &input,
                   StateMachine<IGameState> &stateMachine,
                   Entity &player,
                   EventQueue &events,
                   bool *openFlag = nullptr); // GameplayState passes &inventoryOpen_ to guard re-push; PauseState leaves default, doesn't need it since Pause already blocks GameplayState's Update

    void OnEnter() override;
    void OnExit() override; // clears *openFlag_ if set, so GameplayState knows it's safe to reopen
    void Update(float dt) override;
    void Render(RenderSystem &renderSystem, float dt) override;
    // bool BlocksUpdateBelow() const override { return false; }

private:
    void RefreshOptions();

    InputManager &input_;
    StateMachine<IGameState> &stateMachine_;
    Entity &player_;
    EventQueue &events_;
    bool *openFlag_; // non-owning, points at caller's tracking bool if given

    KeyBindings<MenuContext> navInput_;
    Panel<InventoryActionContext> panel_;

    static sf::Font s_font_;
    const sf::Font &font_;
};