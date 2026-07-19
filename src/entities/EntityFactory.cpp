#include "entities/EntityFactory.h"
#include "component/PositionComponent.h"
#include "component/RenderComponent.h"
#include "component/CollisionComponent.h"
#include "component/InteractableComponent.h"
#include "system/GameConstants.h"
#include "log/Logger.h"

namespace EntityFactory
{
    Result<void, AssetError> populate(Registry &registry, const AssetDatabase &db, EntityID id,
                                      const std::string &archetypeId, float x, float y, RenderLayer layer)
    {
        LOG_DEBUG(std::format("Populating entity {} with archetype: {}", id.index, archetypeId));
        const ArchetypeDefinition *archetype = db.findArchetype(archetypeId);
        if (!archetype)
        {
            AssetError err{AssetErrorCode::UnknownId, archetypeId, "unknown archetype"};
            LOG_ERROR(std::format("EntityFactory: {}", err.toString()));
            return Result<void, AssetError>::Err(std::move(err));
        }

        registry.add<PositionComponent>(id, x, y);

        if (archetype->renderId)
        {
            const RenderAssetMetadata *render = db.findRender(*archetype->renderId);
            if (render)
            {
                registry.add<RenderComponent>(id,
                                              render->name, render->RenderData.texturePath, render->RenderData.textureRect,
                                              render->RenderData.sourceTileSize, x, y, layer,
                                              render->RenderData.renderScale);
            }
            else
            {
                // the entity was created without anyone
                // being told its render reference didn't resolve. Now
                // logged once, at the point that actually knows both the
                // archetype and the dangling id.
                LOG_WARNING(std::format(
                    "EntityFactory: archetype '{}' references unknown render id '{}' — entity {} created without a RenderComponent",
                    archetypeId, *archetype->renderId, id.index));
            }
        }

        if (archetype->collisionId)
        {
            const CollisionAssetMetadata *collision = db.findCollision(*archetype->collisionId);
            if (collision)
            {
                float scale = 1.f;
                if (archetype->renderId)
                {
                    if (const RenderAssetMetadata *render = db.findRender(*archetype->renderId))
                        scale = render->RenderData.renderScale;
                }
                registry.add<CollisionComponent>(id,
                                                 collision->data.offsetX * scale, collision->data.offsetY * scale,
                                                 collision->data.width * scale, collision->data.height * scale);
            }
            else
            {
                LOG_WARNING(std::format(
                    "EntityFactory: archetype '{}' references unknown collision id '{}' — entity {} created without a CollisionComponent",
                    archetypeId, *archetype->collisionId, id.index));
            }
        }

        if (archetype->interactionId)
        {
            const InteractionAssetMetadata *interaction = db.findInteraction(*archetype->interactionId);
            if (interaction)
            {
                registry.add<InteractableComponent>(id, *archetype->interactionId);
            }
            else
            {
                LOG_WARNING(std::format(
                    "EntityFactory: archetype '{}' references unknown interaction id '{}' — entity {} created without an InteractableComponent",
                    archetypeId, *archetype->interactionId, id.index));
            }
        }

        return Result<void, AssetError>::Ok();
    }
}