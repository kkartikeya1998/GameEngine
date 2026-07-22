#pragma once
#include <string>
#include "asset/metadata/AssetMetadataName.h"
#include "external/json.hpp"

enum class ItemCategory { Medicine, PokeBall, KeyItem, TM, Berry, HeldItem, Misc };

// Catalog entry for one item type. Inventories store itemId + quantity only —
// this is display name, description, icon,
// stacking rules, and what using the item does.
struct ItemMetadata : public AssetMetadataName {
    std::string description;
    ItemCategory category = ItemCategory::Misc;
    std::string icon;            // key into render/atlas lookup, same convention as RenderAssetMetadata's sprite key
    int maxStack = 99;
    int healAmount = 0;          // 0 = item has no direct HP effect (e.g. key items, poke balls)
    std::string interactionId;   // empty = item has no direct "use" (e.g. a plot key item); non-empty -> InteractionManager
    static ItemMetadata fromJson(const std::string& id, const nlohmann::json& entry);
};