#pragma once
#include "asset/AssetDatabase.h"
#include "entities/Registry.h"
#include "render/RenderLayer.h"

namespace EntityFactory {
    // Populates `id` (already created via registry.create()) with whichever
    // components the archetype references — an entity with no animationId
    // simply doesn't get an AnimationComponent. Throws std::runtime_error
    // if archetypeId is unknown; caller owns the created id either way.
    void populate(Registry& registry, const AssetDatabase& db, EntityID id,
                   const std::string& archetypeId,
                   float x, float y, RenderLayer layer = RenderLayer::Terrain);
}