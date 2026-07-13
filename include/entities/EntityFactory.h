#pragma once
#include <memory>
#include "asset/AssetDatabase.h"
#include "entities/Entity.h"
#include "render/RenderLayer.h"

namespace EntityFactory {
    // Builds an entity from an archetype id + world position. Pulls
    // whichever components the archetype actually references — an
    // entity with no animationId simply doesn't get an AnimationComponent.
    std::unique_ptr<Entity> make(const AssetDatabase& db, const std::string& archetypeId,
                                  float x, float y, RenderLayer layer = RenderLayer::Terrain);
}