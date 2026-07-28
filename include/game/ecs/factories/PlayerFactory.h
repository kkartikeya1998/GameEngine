#pragma once

#include "engine/ecs/Registry.h"
#include "game/ecs/movement/PositionComponent.h"
#include "game/ecs/movement/VelocityComponent.h"
#include "game/ecs/movement/DirectionComponent.h"
#include "game/ecs/movement/FreeMovementComponent.h"
#include "game/ecs/collision/CollisionComponent.h"
#include "game/ecs/movement/MovementStateComponent.h"


#include "game/ecs/player/PlayerControlComponent.h"
#include "game/ecs/inventory/InventoryComponent.h"
#include "game/ecs/tbd/HealthComponent.h"
#include "game/GameConstants.h"
#include "engine/ecs/EntityFactory.h"
#include "engine/assets/AssetPaths.h"
#include "engine/assets/AssetDatabase.h"

inline EntityID makePlayer(
    Registry &registry,
    const AssetDatabase &assets,
    float x, float y,
    float movement_speed = GameConstants::PLAYER_SPEED,
    float sprintMultiplier = 1.5f,
    int maxHP = 100)
{
    EntityID id = registry.create();

    EntityFactory::populate(registry, assets, id, "player", x, y, RenderLayer::Characters);

    registry.add<VelocityComponent>(id);
    registry.add<DirectionComponent>(id, Direction::DOWN);
    registry.add<FreeMovementComponent>(id, movement_speed, sprintMultiplier);
    registry.add<MovementStateComponent>(id);
    registry.add<InventoryComponent>(id);
    registry.add<PlayerControlComponent>(id);
    registry.add<HealthComponent>(id, maxHP, maxHP);

    return id;
}