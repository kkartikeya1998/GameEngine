#pragma once

#include <unordered_set>
#include "entities/Registry.h"
#include "entities/EntityFactory.h"
#include "component/PositionComponent.h"
#include "component/VelocityComponent.h"
#include "component/DirectionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/AnimationComponent.h"
#include "component/MovementStateComponent.h"
#include "entities/npc/WanderAIComponent.h"
#include "entities/npc/WildSpawnComponent.h"
#include "entities/npc/TileRestrictionComponent.h"
#include "asset/AssetDatabase.h"

// WildPokemon.h
#include "component/AnimationComponent.h"   // replaces WalkCycleTimer include

inline EntityID makeWildPokemon(
    Registry &registry, int speciesId, int level, float x, float y,
    const AssetDatabase &assets,
    std::unordered_set<TileCoord, TileCoordHash> allowedTiles = {})
{
    EntityID id = registry.create();
    std::string archetypeId = "species" + std::to_string(speciesId);

    Result<void, AssetError> result =
        EntityFactory::populate(registry, assets, id, archetypeId, x, y, RenderLayer::Characters);

    if (!result)
    {
        // populate() already logged the specific failure (unknown
        // archetype). We still add PositionComponent so this entity
        // isn't left in the broken half-state the old catch() block
        // silently produced — MovementSystem, VelocityIntegrationSystem,
        // etc. all early-out on missing PositionComponent otherwise.
        registry.add<PositionComponent>(id, x, y);
    }

    registry.add<VelocityComponent>(id);
    registry.add<DirectionComponent>(id, Direction::DOWN);
    registry.add<MovementStateComponent>(id);
    registry.add<WanderAIComponent>(id);
    registry.add<WildSpawnComponent>(id, speciesId, level);
    // AnimationComponent added by populate() when the archetype
    // declares pmdAnimation (species1) or animation. species4 has
    // neither in species_metadatav2.json, so it gets no animation
    // player and AnimationSystem's view<> simply skips it — same as
    // any entity lacking the component today.

    if (!allowedTiles.empty())
        registry.add<TileRestrictionComponent>(id, std::move(allowedTiles));

    return id;
}