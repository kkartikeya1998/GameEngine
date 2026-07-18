#include "system/ItemPickupSystem.h"
#include "component/InventoryComponent.h"
#include "component/WorldItemComponent.h"
#include "system/InventorySystem.h"
#include "events/Events.h"

void ItemPickupSystem::pickup(Registry& registry, const AssetDatabase& assets, EventQueue& events,
                               EntityID player, EntityID worldItem) {
    auto* item = registry.get<WorldItemComponent>(worldItem);
    auto* inv = registry.get<InventoryComponent>(player);
    if (!item || !inv) return;

    InventorySystem::addItem(*inv, assets, item->itemId, item->quantity);
    events.Push(ItemPickedUp{player, item->itemId, item->quantity});
    registry.destroy(worldItem);
}