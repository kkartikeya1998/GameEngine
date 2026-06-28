#pragma once

// ---------------------------------------------------------------------------
// TransformComponent — world-pixel-space origin for entities that don't
// need a full movement component (chiefly MapObject-turned-Entity).
//
// MapObject's old originX_/originY_ fields, unchanged in meaning:
// "Origin is already center-bottom" (per existing notes) — this is the
// world-pixel center-bottom anchor, NOT re-derived from sprite
// dimensions anywhere downstream.
//
// Player/NPC entities do NOT use TransformComponent — their position
// lives on GridMovementComponent/FreeMovementComponent instead, since
// they need facing/speed/hitbox alongside position, not just a static
// point. TransformComponent exists specifically for entities with a
// FIXED position and nothing else movement-shaped.
// ---------------------------------------------------------------------------
struct TransformComponent {
    int originX = 0;
    int originY = 0;
};