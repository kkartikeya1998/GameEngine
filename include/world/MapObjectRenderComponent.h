#pragma once

#include "asset/MapObjectRepository.h"

// ---------------------------------------------------------------------------
// MapObjectRenderComponent — replaces the MapObject class as the holder
// of everything its constructor used to take.
//
// IMPORTANT CAVEAT (flagging rather than guessing): MapObject.h only
// showed me method SIGNATURES (getTypeName(), getTexturePath(),
// getFootprint(), getCollisionBox(), getTeleportTarget()), not their
// IMPLEMENTATIONS. getCollisionBox() in particular is documented as
// doing real unit-conversion work ("resolves... using tileSize to
// convert the metadata's tile-fraction units into pixels") — that is
// LOGIC, not a passthrough, and per "verify before declaring done" I
// should not invent what ObjectTypeMetadata's fields are named or how
// that conversion is written.
//
// So this component carries exactly what MapObject's constructor took
// (metadata pointer + origin + teleport target), unchanged. The
// FUNCTIONS that used to be MapObject's methods move to
// MapObjectSystem.h as free functions taking `const
// MapObjectRenderComponent&` — but their BODIES still need
// MapObject.cpp (not yet seen) to port correctly. Until that's shared,
// MapObjectSystem's getCollisionBox()/getFootprint() are stubbed with
// a clear marker rather than guessed at.
// ---------------------------------------------------------------------------
struct MapObjectRenderComponent {
    const ObjectTypeMetadata* metadata = nullptr;
    int originX = 0;
    int originY = 0;
    int teleportTargetMapId = -1;
    int teleportTargetX = 0;
    int teleportTargetY = 0;
};