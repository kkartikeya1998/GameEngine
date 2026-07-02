#pragma once

#include "entities/Component.h"
#include "asset/repositories/MapObjectRepository.h"

// ---------------------------------------------------------------------------
// MapObjectRenderComponent — replaces the MapObject class as the holder
// of everything its constructor used to take, minus position.
//
// CHANGED (PositionComponent pass): originX, originY are GONE from
// this struct — they now live on a sibling PositionComponent, held
// separately on the same Entity (see Entity::add<PositionComponent>()
// in MapLoader.cpp's map-object construction site). MapObjectSystem's
// free functions that used to read render.originX/originY (e.g.
// getOriginPixelX/Y, getCollisionBox) now need to take a
// PositionComponent& alongside this component instead — that change
// lives in MapObjectSystem.h/.cpp, not yet seen, so its body isn't
// ported here; only this component's own shape is.
// ---------------------------------------------------------------------------
struct MapObjectRenderComponent : public Component {
    const ObjectTypeMetadata* metadata = nullptr;

    MapObjectRenderComponent() = default;

    MapObjectRenderComponent(
        const ObjectTypeMetadata* metadata
    )
        : metadata(metadata)
    {}
};