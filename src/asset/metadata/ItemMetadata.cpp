#include "asset/metadata/ItemMetadata.h"
#include <stdexcept>

namespace {
ItemCategory categoryFromString(const std::string& s) {
    if (s == "medicine")  return ItemCategory::Medicine;
    if (s == "pokeball")  return ItemCategory::PokeBall;
    if (s == "key_item")  return ItemCategory::KeyItem;
    if (s == "tm")        return ItemCategory::TM;
    if (s == "berry")     return ItemCategory::Berry;
    if (s == "held_item") return ItemCategory::HeldItem;
    return ItemCategory::Misc;
}
}

ItemMetadata ItemMetadata::fromJson(const std::string& id, const nlohmann::json& entry) {
    ItemMetadata m;
    m.name        = entry.value("displayName", id);
    m.description = entry.value("description", "");
    m.category    = categoryFromString(entry.value("category", "misc"));
    m.icon        = entry.value("icon", "");
    m.maxStack    = entry.value("maxStack", 99);
    m.healAmount  = entry.value("healAmount", 0);
    m.interactionId = entry.value("interactionId", "");
    return m;
}