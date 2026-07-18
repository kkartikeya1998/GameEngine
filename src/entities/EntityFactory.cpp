#include "entities/EntityFactory.h"
#include "component/PositionComponent.h"
#include "component/RenderComponent.h"
#include "component/CollisionComponent.h"
#include "component/InteractableComponent.h"
#include "system/GameConstants.h"
#include "log/Logger.h"
#include <stdexcept>

namespace EntityFactory
{

    void populate(Registry &registry, const AssetDatabase &db, EntityID id,
                  const std::string &archetypeId, float x, float y, RenderLayer layer)
    {
        LOG_DEBUG("Populating entity " + std::to_string(id.index) + " with archetype: " + archetypeId);
        const ArchetypeDefinition *archetype = db.findArchetype(archetypeId);
        if (!archetype)
        {
            LOG_ERROR("EntityFactory: unknown archetype: " + archetypeId);
            throw std::runtime_error("EntityFactory: unknown archetype: " + archetypeId);
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
                    {
                        scale = render->RenderData.renderScale;
                    }
                }
                registry.add<CollisionComponent>(id,
                                                 collision->data.offsetX * scale, collision->data.offsetY * scale,
                                                 collision->data.width * scale, collision->data.height * scale);
            }
        }

        if (archetype->interactionId)
        {
            const InteractionAssetMetadata *interaction = db.findInteraction(*archetype->interactionId);
            if (interaction)
            {
                registry.add<InteractableComponent>(id, *archetype->interactionId);
            }
        }
    }
}