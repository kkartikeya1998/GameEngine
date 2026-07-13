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
std::optional<AABB> MapObject::getCollisionData() const
{
    if (!metadata_->CollisionData)
        return std::nullopt;

    const CollisionData& cb = *metadata_->CollisionData;

    float scale = GameConstants::TILE_SIZE / static_cast<float>(metadata_->sourceTileSize);

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