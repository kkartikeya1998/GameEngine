#pragma once
#include <string>

// ---------------------------------------------------------------------------
// ITeleportable — capability interface for tiles that transport the player
// to another map (doors, cave entrances, warp pads).
// ---------------------------------------------------------------------------
class ITeleportable {
public:
    virtual ~ITeleportable() = default;
    virtual int getTargetMapId() const = 0;
    virtual void getTargetPosition(int& outX, int& outY) const = 0;
};