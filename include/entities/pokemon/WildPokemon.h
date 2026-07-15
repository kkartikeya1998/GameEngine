#pragma once

#include <unordered_set>
#include "entities/Registry.h"
#include "entities/EntityFactory.h"
#include "component/PositionComponent.h"
#include "component/VelocityComponent.h"
#include "component/DirectionComponent.h"
#include "component/MovementStateComponent.h"
#include "entities/npc/WanderAIComponent.h"
#include "entities/npc/WildSpawnComponent.h"
#include "entities/npc/TileRestrictionComponent.h"
#include "asset/AssetDatabase.h"

inline EntityID makeWildPokemon(
    Registry& registry,
    int speciesId,
    int level,
    float x, float y,
    const AssetDatabase &assets,
    std::unordered_set<TileCoord, TileCoordHash> allowedTiles = {})
{
    EntityID id = registry.create();
    std::string archetypeId = "species_" + std::to_string(speciesId);

    try
    {
        EntityFactory::populate(registry, assets, id, archetypeId, x, y, RenderLayer::Characters);
    }
    catch (const std::runtime_error &)
    {
        // Missing species: preserve old graceful-degradation behavior —
        // entity exists with AI/spawn data but no sprite/collision.
        registry.add<PositionComponent>(id, x, y);
    }

    registry.add<VelocityComponent>(id);
    registry.add<DirectionComponent>(id, Direction::DOWN);
    registry.add<MovementStateComponent>(id);
    registry.add<WanderAIComponent>(id);
    registry.add<WildSpawnComponent>(id, speciesId, level);

    if (!allowedTiles.empty())
        registry.add<TileRestrictionComponent>(id, std::move(allowedTiles));

    return id;
}