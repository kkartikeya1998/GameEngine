#include "system/MapObjectSystem.h"
#include "system/GameConstants.h"

namespace MapObjectSystem {

const std::string& getTypeName(const MapObjectRenderComponent& component)
{
    // Ported verbatim from MapObject::getTypeName() — note this reads
    // metadata->name, not a "typeName" field.
    return component.metadata->name;
}

const std::string& getTexturePath(const MapObjectRenderComponent& component)
{
    return component.metadata->texturePath;
}

const std::vector<FootprintCell>& getFootprint(const MapObjectRenderComponent& component)
{
    return component.metadata->footprint;
}

int getOriginPixelX(const MapObjectRenderComponent& component)
{
    return component.originX;
}

int getOriginPixelY(const MapObjectRenderComponent& component)
{
    return component.originY;
}

int getTeleportTargetMapId(const MapObjectRenderComponent& component)
{
    return component.teleportTargetMapId;
}

void getTeleportTarget(const MapObjectRenderComponent& component, int& x, int& y)
{
    x = component.teleportTargetX;
    y = component.teleportTargetY;
}

std::optional<AABB> getCollisionBox(const MapObjectRenderComponent& component)
{
    if (!component.metadata->collisionBox)
        return std::nullopt;

    const CollisionBox& cb = *component.metadata->collisionBox;

    // Ported verbatim from MapObject::getCollisionBox(). collisionBox
    // is authored in native-pixel space (same space as the sprite's
    // own textureRect) — scale converts that into world pixels using
    // the same ratio SFMLRenderer::drawMapObject uses to scale the
    // sprite itself, so the hitbox always matches the drawn sprite
    // size automatically.
    float scale = GameConstants::TILE_SIZE / static_cast<float>(component.metadata->sourceTileSize);

    // origin (originX, originY) IS ALREADY the sprite's center-bottom
    // anchor in world pixels. Do NOT re-add scaled native w/h here —
    // that would double-count the anchor. Only cb.offsetX/offsetY
    // (native-pixel displacement FROM that anchor) get scaled and
    // added.
    float offsetWorldX = cb.offsetX * scale;
    float offsetWorldY = cb.offsetY * scale;

    float bottomCenterWorldX = component.originX + offsetWorldX;
    float bottomCenterWorldY = component.originY + offsetWorldY;

    float worldX = bottomCenterWorldX - (cb.width  * scale) / 2.f;
    float worldY = bottomCenterWorldY - (cb.height * scale);

    return AABB{
        worldX,
        worldY,
        cb.width  * scale,
        cb.height * scale
    };
}

} // namespace MapObjectSystem