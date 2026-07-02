#pragma once

#include <optional>
#include <string>
#include <vector>

#include "world/MapObjectRenderComponent.h"
#include "asset/repositories/MapObjectRepository.h"
// for AABB and direction juagad
#include "tmp/movement/PositionComponent.h"

// ---------------------------------------------------------------------------
// MapObjectSystem — free functions holding the logic that used to live
// inside MapObject's methods (MapObject.cpp, now fully seen — this is
// a verbatim port, not a reconstruction from documentation).
//
// MapObject is GONE as a class. MapObjectRenderComponent is pure data
// (metadata pointer + origin + teleport target, exactly what
// MapObject's constructor took) — these functions are where its
// METHOD BODIES moved to, ported field-for-field.
//
// All functions take `const MapObjectRenderComponent&` instead of
// being methods on MapObject — callers that used to write
// `mapObject->getTypeName()` now write
// `MapObjectSystem::getTypeName(*component)`.
// ---------------------------------------------------------------------------
namespace MapObjectSystem {

// Ported from MapObject::getTypeName(). NOTE: this returns
// metadata->name, NOT metadata->typeName, despite the method's own
// name — confirmed against the real MapObject.cpp body, not guessed.
// (An earlier draft of MapObjectRenderComponent's consumer code in
// RenderSystem.cpp incorrectly assumed a `typeName` field existed on
// ObjectTypeMetadata — that was a guess made before MapObject.cpp was
// available, and is wrong; this function is the corrected source of
// truth going forward.)
const std::string& getTypeName(const MapObjectRenderComponent& component);

const std::string& getTexturePath(const MapObjectRenderComponent& component);

const std::vector<FootprintCell>& getFootprint(const MapObjectRenderComponent& component);

std::optional<AABB> getCollisionBox(const MapObjectRenderComponent& component);

} // namespace MapObjectSystem