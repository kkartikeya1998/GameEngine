#pragma once

#include "entities/Component.h"

// ---------------------------------------------------------------------------
// FreeMovementComponent — pure data for continuous, axis-separated
// movement.
//
// CHANGED (PositionComponent pass): x, y, facing are GONE from this
// struct — they now live on a sibling PositionComponent, held
// separately on the same Entity (see Entity::add<PositionComponent>()
// call sites in Player.h / MapLoader.cpp / wherever NPCs are built).
// This struct keeps only the fields that are genuinely specific to
// "how this entity moves" rather than "where it currently is."
//
// MovementSystem::updateFree/getFreeHitbox/freeHitboxAt now take a
// PositionComponent& alongside this component — see MovementSystem.h.
//
// Field-for-field, this is what survives from the previous version:
//   speed                                   -> speed
//   hitboxWidth/hitboxHeight                -> hitboxWidth/hitboxHeight
//   hitboxOffsetX/hitboxOffsetY              -> hitboxOffsetX/hitboxOffsetY
//   isMoving                                 -> isMoving
// (x, y, facing moved out to PositionComponent.)
// ---------------------------------------------------------------------------
struct FreeMovementComponent : public Component {
    float speed = 0.f;

    // Hitbox sized smaller than a full tile on purpose — see original
    // FreeMovementMechanics.h comment: using full sprite/tile size as
    // the collision box makes movement feel like it collides with
    // invisible walls around empty space near the sprite's edges.
    float hitboxWidth = 0.f;
    float hitboxHeight = 0.f;
    float hitboxOffsetX = 0.f;
    float hitboxOffsetY = 0.f;

    // True only on a frame where at least one axis actually moved
    // (i.e. not fully blocked on both axes).
    bool isMoving = false;

    FreeMovementComponent() = default;

    FreeMovementComponent(
        float speed,
        float hitboxWidth, float hitboxHeight,
        float hitboxOffsetX, float hitboxOffsetY,
        bool isMoving = false
    )
        : speed(speed)
        , hitboxWidth(hitboxWidth), hitboxHeight(hitboxHeight)
        , hitboxOffsetX(hitboxOffsetX), hitboxOffsetY(hitboxOffsetY)
        , isMoving(isMoving)
    {}
};