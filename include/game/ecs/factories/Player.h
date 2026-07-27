#pragma once

#include "engine/ecs/Registry.h"
#include "game/ecs/actor/PositionComponent.h"
#include "game/ecs/actor/VelocityComponent.h"
#include "game/ecs/actor/DirectionComponent.h"
#include "game/ecs/actor/FreeMovementComponent.h"
#include "game/ecs/actor/CollisionComponent.h"
#include "component/MovementStateComponent.h"


#include "component/PlayerControlComponent.h"
#include "component/InventoryComponent.h"
#include "component/HealthComponent.h"
#include "system/GameConstants.h"
#include "engine/ecs/EntityFactory.h"
#include "engine/assets/AsssetPaths.h"
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