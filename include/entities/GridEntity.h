#pragma once

#include <memory>

#include "entities/Entity.h"
#include "entities/movement/IMovementMechanics.h"

// ---------------------------------------------------------------------------
// GridEntity — discrete, tile-snapped movement. This is the exact
// behavior the old Entity class had: move(dir) commits a full tile-hop
// synchronously, AnimationComponent lerps the visual position from the
// old tile to the new one over time.
//
// Completely unchanged in behavior from before the Entity split — only
// renamed and now deriving from the smaller Entity base instead of being
// the only Entity. Any existing GridMovementMechanics-based entity
// (today's Player, or a future grid-based NPC) becomes a GridEntity with
// zero behavior change.
// ---------------------------------------------------------------------------
class GridEntity : public Entity {
public:
    explicit GridEntity(std::unique_ptr<IMovementMechanics> movement);

    // movement request — discrete, commits a full tile-hop
    virtual void move(Direction dir);

    int getX() const;
    int getY() const;

    Direction getDirection() const override;
    float getRenderX() const override;
    float getRenderY() const override;

    IMovementMechanics& movement();
    const IMovementMechanics& movement() const;

protected:
    std::unique_ptr<IMovementMechanics> movement_;
};