#pragma once

#include <unordered_set>
#include "engine/ecs/Registry.h"
#include "engine/ecs/EntityFactory.h"
#include "game/ecs/movement/PositionComponent.h"
#include "game/ecs/movement/VelocityComponent.h"
#include "game/ecs/movement/DirectionComponent.h"
#include "game/ecs/movement/MovementStateComponent.h"
#include "game/ecs/animation/AnimationComponent.h"
#include "game/ecs/movement/MovementStateComponent.h"
#include "game/ecs/npc/WanderAIComponent.h"
#include "game/ecs/npc/WildSpawnComponent.h"
#include "game/ecs/npc/TileRestrictionComponent.h"
#include "engine/assets/AssetDatabase.h"
#include "game/ecs/animation/AnimationComponent.h"

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