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
int MapObject::getOriginX() const
{
    return originX_;
}

int MapObject::getOriginY() const
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