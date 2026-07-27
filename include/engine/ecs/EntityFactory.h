#pragma once
#include "engine/assets/AssetDatabase.h"
#include "engine/assets/AssetError.h"
#include "engine/utility/Result.h"
#include "engine/ecs/Registry.h"
#include "engine/render/RenderLayer.h"

namespace EntityFactory
{
    // Populates `id` (already created via registry.create()) with whichever
    // components the archetype references. Fails (Result::Err) only when
    // archetypeId itself is unknown — that's the one case where the caller
    // has nothing safe left to do with `id`. A referenced render/collision/
    // interaction id that fails to resolve is logged as a warning and
    // skipped instead: the entity is still created, just missing that one
    // component, so a single bad reference in content doesn't cascade into
    // losing the whole entity. Caller owns `id` either way — on Err it's
    // still the caller's job to destroy() it, same as before.
    Result<void, AssetError> populate(Registry &registry, const AssetDatabase &db, EntityID id,
                                       const std::string &archetypeId,
                                       float x, float y, RenderLayer layer = RenderLayer::Terrain);
}