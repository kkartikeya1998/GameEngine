#include "system/InventorySystem.h"
#include "entities/Registry.h"
#include "component/HealthComponent.h"
#include "component/PositionComponent.h"
#include "component/TextRenderComponent.h"
#include "component/LifetimeComponent.h"
#include "component/FloatingTextComponent.h"
#include "system/GameConstants.h"
#include "log/Logger.h"
#include <algorithm>

int InventorySystem::addItem(InventoryComponent& inv, const AssetDatabase& assets,
                             const std::string& itemId, int quantity)
{
    LOG_INFO("Attempting to add item: " + itemId);
    const ItemMetadata* meta = assets.findItem(itemId);
    int maxStack = meta ? meta->maxStack : 99;

    for (auto& stack : inv.items) {
        if (stack.itemId != itemId) continue;
        int room = maxStack - stack.quantity;
        int take = std::min(room, quantity);
        stack.quantity += take;
        quantity -= take;
        if (quantity == 0) return 0;
    }
    while (quantity > 0) {
        int take = std::min(maxStack, quantity);
        inv.items.push_back(ItemStack{itemId, take});
        quantity -= take;
    }
    return 0;
}

int InventorySystem::removeItem(InventoryComponent& inv, const std::string& itemId, int quantity) {
    int removed = 0;
    LOG_INFO("Attempting to remove item: " + itemId + ", quantity: " + std::to_string(quantity));
    for (auto it = inv.items.begin(); it != inv.items.end() && quantity > 0; ) {
        if (it->itemId != itemId) { ++it; continue; }
        int take = std::min(it->quantity, quantity);
        it->quantity -= take;
        quantity -= take;
        removed += take;
        it = (it->quantity == 0) ? inv.items.erase(it) : it + 1;
    }
    return removed;
}

bool InventorySystem::hasItem(const InventoryComponent& inv, const std::string& itemId) {
    return countItem(inv, itemId) > 0;
}

int InventorySystem::countItem(const InventoryComponent& inv, const std::string& itemId) {
    int total = 0;
    for (const auto& s : inv.items)
        if (s.itemId == itemId) total += s.quantity;
    return total;
}

void InventorySystem::handleItemConsumed(Registry& registry, const AssetDatabase& assets, const ItemConsumed& e) {
    if (auto* inv = registry.get<InventoryComponent>(e.owner))
        removeItem(*inv, e.itemId, 1);

    const ItemMetadata* meta = assets.findItem(e.itemId);
    if (!meta || meta->healAmount <= 0)
        return; // not a healing item — nothing further to do

    auto* health = registry.get<HealthComponent>(e.owner);
    if (!health) {
        LOG_WARNING("InventorySystem: ItemConsumed healAmount item used by entity " +
                    std::to_string(e.owner.index) + " with no HealthComponent — heal dropped");
        return;
    }

    health->Heal(meta->healAmount);

    // Short-lived "+N HP" popup above the entity. FloatingTextSystem tracks
    // the owner's position, drifts it upward, and fades it out each frame;
    // LifetimeSystem still owns destroying it once its time runs out.
    if (auto* ownerPos = registry.get<PositionComponent>(e.owner)) {
        constexpr float kPopupDuration = 1.2f;
        constexpr float kPopupRiseSpeed = 30.f;
        sf::Vector2f baseOffset{0.f, -GameConstants::TILE_SIZE * 0.5f}; // starts close to the player, not a full tile up

        EntityID popup = registry.create();
        registry.add<PositionComponent>(popup, ownerPos->x + baseOffset.x, ownerPos->y + baseOffset.y);
        registry.add<TextRenderComponent>(popup, "+" + std::to_string(meta->healAmount) + " HP",
                                          sf::Color::Green, 16u, RenderLayer::ForegroundEffects);
        registry.add<LifetimeComponent>(popup, kPopupDuration);
        registry.add<FloatingTextComponent>(popup, e.owner, baseOffset, kPopupRiseSpeed, kPopupDuration);
    }
}