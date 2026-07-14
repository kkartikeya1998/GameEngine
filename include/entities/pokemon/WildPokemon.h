#pragma once

#include <unordered_set>
#include "entities/Entity.h"
#include "entities/EntityFactory.h"
#include "component/PositionComponent.h"
#include "component/VelocityComponent.h"
#include "component/DirectionComponent.h"
#include "component/MovementStateComponent.h"
#include "entities/npc/WanderAIComponent.h"
#include "entities/npc/WildSpawnComponent.h"
#include "entities/npc/TileRestrictionComponent.h"
#include "asset/AssetDatabase.h"

inline Entity makeWildPokemon(
    int speciesId,
    int level,
    float x, float y,
    const AssetDatabase &assets,
    std::unordered_set<TileCoord, TileCoordHash> allowedTiles = {})
{
    std::string archetypeId = "species_" + std::to_string(speciesId);

    std::unique_ptr<Entity> base;
    try
    {
        base = EntityFactory::make(assets, archetypeId, x, y, RenderLayer::Characters);
    }
    catch (const std::runtime_error &)
    {
        // Missing species: preserve old graceful-degradation behavior —
        // entity exists with AI/spawn data but no sprite/collision.
        base = std::make_unique<Entity>();
        base->add<PositionComponent>(x, y);
    }

    Entity e = std::move(*base);
    e.add<VelocityComponent>();
    e.add<DirectionComponent>(Direction::DOWN);
    e.add<MovementStateComponent>();
    e.add<WanderAIComponent>();
    e.add<WildSpawnComponent>(speciesId, level);

    if (!allowedTiles.empty())
        e.add<TileRestrictionComponent>(std::move(allowedTiles));

    return e;
}