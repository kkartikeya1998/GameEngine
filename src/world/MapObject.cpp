#include "world/MapObject.h"

// -------------------------
// Constructor
// -------------------------
MapObject::MapObject(
    const ObjectTypeMetadata* metadata,
    int originX,
    int originY,
    int teleportTargetMapId,
    int teleportTargetX,
    int teleportTargetY
)
    : metadata_(metadata)
    , originX_(originX)
    , originY_(originY)
    , teleportTargetMapId_(teleportTargetMapId)
    , teleportTargetX_(teleportTargetX)
    , teleportTargetY_(teleportTargetY)
{
}

// -------------------------
// Metadata access
// -------------------------
const std::string& MapObject::getTypeName() const
{
    return metadata_->name;
}

const std::string& MapObject::getTexturePath() const
{
    return metadata_->texturePath;
}

const std::vector<FootprintCell>& MapObject::getFootprint() const
{
    return metadata_->footprint;
}

// -------------------------
// Position
// -------------------------
int MapObject::getOriginPixelX() const
{
    return originX_;
}

int MapObject::getOriginPixelY() const
{
    return originY_;
}

// -------------------------
// Teleport data
// -------------------------
int MapObject::getTeleportTargetMapId() const
{
    return teleportTargetMapId_;
}

void MapObject::getTeleportTarget(int& x, int& y) const
{
    x = teleportTargetX_;
    y = teleportTargetY_;
}

// -------------------------
// Collision
// -------------------------
std::optional<AABB> MapObject::getCollisionBox(float tileSize) const
{
    if (!metadata_->collisionBox)
        return std::nullopt;

    const CollisionBox& cb = *metadata_->collisionBox;

    // collision_box is authored in the SAME native-pixel space as the
    // sprite's own textureRect (w/h) — see CollisionBox's doc comment in
    // MapObjectRepository.h. scale converts that native space into world
    // pixels, using the exact same ratio SFMLRenderer::drawMapObject uses
    // to scale the sprite itself — so the hitbox always matches however
    // big the sprite is currently being drawn, automatically.
    float scale = tileSize / static_cast<float>(metadata_->sourceTileSize);

    // origin_ (originX_, originY_) IS ALREADY the sprite's center-bottom
    // point in world pixels — see SFMLRenderer::drawMapObject, which
    // treats origin as exactly that anchor and never re-derives it from
    // native w/h. So collision must match: do NOT re-add nativeW/2 or
    // nativeH here — doing so would double-count the anchor (origin
    // already encodes "center-bottom"; adding scaled native dimensions
    // on top shifts the box away from where the sprite is actually
    // drawn). Only cb.offsetX/offsetY (native-pixel displacement FROM
    // that point) get scaled and added.
    float offsetWorldX = cb.offsetX * scale;
    float offsetWorldY = cb.offsetY * scale;

    float bottomCenterWorldX = originX_ + offsetWorldX;
    float bottomCenterWorldY = originY_ + offsetWorldY;

    float worldX = bottomCenterWorldX - (cb.width  * scale) / 2.f;
    float worldY = bottomCenterWorldY - (cb.height * scale);

    return AABB{
        worldX,
        worldY,
        cb.width  * scale,
        cb.height * scale
    };
}