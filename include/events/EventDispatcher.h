#pragma once

#include "events/EventQueue.h"
#include "game/IGameContext.h"

class AssetDatabase;
class InteractionManager;


class EventDispatcher
{
public:

    EventDispatcher(
        EventQueue& events,
        AssetDatabase& assets,
        IGameContext& context,
        InteractionManager& interactions);


    void Process();


private:

    EventQueue& events_;
    AssetDatabase& assets_;
    IGameContext& context_;
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