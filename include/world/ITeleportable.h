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

class StaticTeleport : public ITeleportable {
public:
    StaticTeleport(int targetMapId, int targetX, int targetY)
        : targetMapId_(targetMapId), targetX_(targetX), targetY_(targetY) {}
 
    int getTargetMapId() const override { return targetMapId_; }
    void getTargetPosition(int& outX, int& outY) const override {
        outX = targetX_;
        outY = targetY_;
    }
 
private:
    int targetMapId_;
    int targetX_, targetY_;
};