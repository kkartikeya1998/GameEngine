#include "asset/metadata/ArchetypeDefinition.h"

ArchetypeDefinition ArchetypeDefinition::fromJson(const std::string& id, const nlohmann::json& entry)
{
    ArchetypeDefinition def;
    def.name = id;
    if (entry.contains("render"))    def.renderId    = entry["render"].get<std::string>();
    if (entry.contains("collision")) def.collisionId = entry["collision"].get<std::string>();
    if (entry.contains("animation")) def.animationId = entry["animation"].get<std::string>();
    return def;
}