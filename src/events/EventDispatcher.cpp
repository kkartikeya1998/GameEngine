#include "engine/events/EventDispatcher.h"

#include "engine/assets/AssetDatabase.h"
#include "interaction/InteractionManager.h"
#include "system/InventorySystem.h"
#include "component/InventoryComponent.h"
#include "engine/ecs/Registry.h"
#include "engine/logging/Logger.h"

#include <string>

EventDispatcher::EventDispatcher(
    EventQueue &events,
    AssetDatabase &assets,
    IGameContext &context,
    InteractionManager &interactions)
    : events_(events),
      assets_(assets),
      context_(context),
      interactions_(interactions)
{
}
void EventDispatcher::Process()
{
    events_.Drain(
        [this](auto &event)
        {
            Handle(event);
        });
}

void EventDispatcher::Handle(ItemConsumed &e)
{
    Registry *registry = context_.GetRegistry();

    if (!registry)
    {
        LOG_WARNING("ItemConsumed without registry");
        return;
    }

    InventorySystem::handleItemConsumed(
        *registry,
        assets_,
        e);
}

void EventDispatcher::Handle(ItemPickedUp &e)
{
    Registry *registry = context_.GetRegistry();

    if (!registry)
    {
        LOG_WARNING("ItemPickedUp without registry");
        return;
    }

    auto *inventory =
        registry->get<InventoryComponent>(e.owner);

    if (!inventory)
    {
        LOG_WARNING("Pickup entity has no inventory");
        return;
    }

    InventorySystem::addItem(
        *inventory,
        assets_,
        e.itemId,
        e.quantity);
}

void EventDispatcher::Handle(InteractionRequested &e)
{
    interactions_.HandleRequested(e);
}

void EventDispatcher::Handle(DialogueFinished &)
{
    interactions_.HandleDialogueFinished({});
}

void EventDispatcher::Handle(BattleFinished &)
{
    interactions_.HandleBattleFinished({});
}

void EventDispatcher::Handle(PlayerJumped &)
{
}

void EventDispatcher::Handle(InventoryOpened &)
{
}

void EventDispatcher::Handle(GamePaused &)
{
}

void EventDispatcher::Handle(GameResumed &)
{
}