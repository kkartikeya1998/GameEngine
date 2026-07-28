#include "game/ecs/interaction/ItemPickupSystem.h"
#include "game/ecs/inventory/InventoryComponent.h"
#include "game/ecs/interaction/WorldItemComponent.h"
#include "game/ecs/inventory/InventorySystem.h"
#include "engine/events/Events.h"

void ItemPickupSystem::pickup(Registry& registry, const AssetDatabase& assets, EventQueue& events,
                               EntityID player, EntityID worldItem) {
    auto* item = registry.get<WorldItemComponent>(worldItem);
    auto* inv = registry.get<InventoryComponent>(player);
    if (!item || !inv) return;

    InventorySystem::addItem(*inv, assets, item->itemId, item->quantity);
    events.Push(ItemPickedUp{player, item->itemId, item->quantity});
    registry.destroy(worldItem);
}