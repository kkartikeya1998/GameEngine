#include "game/events/GameplayEventHandler.h"

#include "engine/assets/AssetDatabase.h"
#include "engine/ecs/Registry.h"
#include "game/ecs/interaction/InteractionManager.h"
#include "game/ecs/inventory/InventorySystem.h"
#include "game/ecs/inventory/InventoryComponent.h"
#include "engine/logging/Logger.h"

GameplayEventHandler::GameplayEventHandler(
    EventQueue& events, AssetDatabase& assets,
    Registry& registry, InteractionManager& interactions)
    : events_(events), assets_(assets), registry_(registry), interactions_(interactions)
{
}

void GameplayEventHandler::Process()
{
    events_.Drain([this](auto& event) { Handle(event); });
}

void GameplayEventHandler::Handle(ItemConsumed& e)
{
    InventorySystem::handleItemConsumed(registry_, assets_, e);
}

void GameplayEventHandler::Handle(ItemPickedUp& e)
{
    auto* inventory = registry_.get<InventoryComponent>(e.owner);
    if (!inventory)
    {
        LOG_WARNING("Pickup entity has no inventory");
        return;
    }
    InventorySystem::addItem(*inventory, assets_, e.itemId, e.quantity);
}

void GameplayEventHandler::Handle(InteractionRequested& e) { interactions_.HandleRequested(e); }
void GameplayEventHandler::Handle(DialogueFinished&)       { interactions_.HandleDialogueFinished({}); }
void GameplayEventHandler::Handle(BattleFinished&)         { interactions_.HandleBattleFinished({}); }
void GameplayEventHandler::Handle(PlayerJumped&)     {}
void GameplayEventHandler::Handle(InventoryOpened&)  {}
void GameplayEventHandler::Handle(GamePaused&)       {}
void GameplayEventHandler::Handle(GameResumed&)      {}