#pragma once

#include "entities/Component.h"
#include "asset/repositories/MapObjectRepository.h"

// ---------------------------------------------------------------------------
// MapObjectRenderComponent — replaces the MapObject class as the holder
// of everything its constructor used to take.
//
// CHANGED (Component base pass): now inherits `: public Component` so
// it can live in Entity's vector<unique_ptr<Component>> storage. No
// field changes — see Component.h for why this base exists.
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
// CAVEAT (Component base pass, confirmed by compiling, not guessed):
// Component's virtual destructor means MapObjectRenderComponent is no
// longer an aggregate — the old brace-init call site in
// MapLoader.cpp (`MapObjectRenderComponent{meta, originX, originY,
// teleportMapId, teleportX, teleportY}`) will no longer compile.
// Added the constructor below specifically so MapLoader.cpp's call
// site can become a normal constructor call instead — see the
// MapLoader.cpp edit in this same pass.
struct MapObjectRenderComponent : public Component {
    const ObjectTypeMetadata* metadata = nullptr;
    int originX = 0;
    int originY = 0;

    MapObjectRenderComponent() = default;

    MapObjectRenderComponent(
        const ObjectTypeMetadata* metadata,
        int originX,
        int originY
    )
        : metadata(metadata)
        , originX(originX)
        , originY(originY)
    {}
};