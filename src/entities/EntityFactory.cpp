#include "entities/EntityFactory.h"
#include "component/PositionComponent.h"
#include "component/RenderComponent.h"
#include "component/CollisionComponent.h"
#include <stdexcept>

namespace EntityFactory {

std::unique_ptr<Entity> make(const AssetDatabase& db, const std::string& archetypeId,
                              float x, float y, RenderLayer layer)
{
    const ArchetypeDefinition* archetype = db.findArchetype(archetypeId);
    if (!archetype)
        throw std::runtime_error("EntityFactory: unknown archetype: " + archetypeId);

    auto entity = std::make_unique<Entity>();
    entity->add<PositionComponent>(x, y);

    if (archetype->renderId) {
        const RenderAssetMetadata* render = db.findRender(*archetype->renderId);
        if (render) {
            entity->add<RenderComponent>(
                render->name, render->RenderData.texturePath, render->RenderData.textureRect,
                render->RenderData.sourceTileSize, x, y, layer);
        }
    }

    if (archetype->collisionId) {
        const CollisionAssetMetadata* collision = db.findCollision(*archetype->collisionId);
        if (collision) {
            float scale = static_cast<float>(GameConstants::TILE_SIZE)
                        / static_cast<float>(db.findRender(*archetype->renderId)->RenderData.sourceTileSize);
            entity->add<CollisionComponent>(
                collision->data.offsetX * scale, collision->data.offsetY * scale,
                collision->data.width * scale, collision->data.height * scale);
        }
    }

    // animationId wiring deferred until you share AnimationComponent

    return entity;
}

}