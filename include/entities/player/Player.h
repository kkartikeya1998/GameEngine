#pragma once

#include "entities/Registry.h"
#include "component/PositionComponent.h"
#include "component/VelocityComponent.h"
#include "component/DirectionComponent.h"
#include "component/FreeMovementComponent.h"
#include "component/CollisionComponent.h"
#include "component/MovementStateComponent.h"
#include "component/WalkCycleTimer.h"
#include "component/RenderComponent.h"
#include "component/PlayerControlComponent.h"
#include "component/InventoryComponent.h"
#include "system/GameConstants.h"
#include "entities/EntityFactory.h"
#include "asset/AsssetPaths.h"
#include "asset/AssetDatabase.h"

inline EntityID makePlayer(
    Registry &registry,
    const AssetDatabase &assets,
    float x, float y,
    float movement_speed = GameConstants::PLAYER_SPEED,
    float sprintMultiplier = 1.5f,
    float walkCyclesPerSecond = 2.0f)
{
    EntityID id = registry.create();

    EntityFactory::populate(registry, assets, id, "player", x, y, RenderLayer::Characters);

    registry.add<VelocityComponent>(id);
    registry.add<DirectionComponent>(id, Direction::DOWN);
    registry.add<FreeMovementComponent>(id, movement_speed, sprintMultiplier);
    registry.add<MovementStateComponent>(id);
    registry.add<WalkCycleTimer>(id, walkCyclesPerSecond);
    registry.add<InventoryComponent>(id);
    registry.add<PlayerControlComponent>(id);

    return id;
}