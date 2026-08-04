#pragma once

#include "engine/events/EventQueue.h"
#include "game/events/GameEvents.h"

class Registry;
class AssetDatabase;
class InteractionManager;

// GameplayEventHandler — game-layer replacement for the old engine/events/EventDispatcher.
// Session-scoped: constructed in GameplayState::OnEnter() alongside GameController,
// destroyed in OnExit() alongside it. Registry& is therefore always valid for its
// entire lifetime — no null-registry guard needed, unlike the old IGameContext path.
class GameplayEventHandler
{
public:
    GameplayEventHandler(EventQueue<GameEvent>& events, AssetDatabase& assets,
                          Registry& registry, InteractionManager& interactions);

    void Process();

private:
    EventQueue<GameEvent>& events_;
    AssetDatabase& assets_;
    Registry& registry_;
    InteractionManager& interactions_;

    void Handle(ItemConsumed& e);
    void Handle(ItemPickedUp& e);
    void Handle(InteractionRequested& e);
    void Handle(DialogueFinished& e);
    void Handle(BattleFinished& e);
    void Handle(PlayerJumped& e);
    void Handle(InventoryOpened& e);
    void Handle(GamePaused& e);
    void Handle(GameResumed& e);
};