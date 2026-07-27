#pragma once

#include "component/SpriteAssetComponent.h"
#include "component/SpriteFrameComponent.h"
#include "game/ecs/actor/MovementMode.h"

#pragma once

enum class TerrainType
{
    Ground,
    Wall,
    Water
};

class Tile
{
public:
    Tile() = default;
    Tile(const Tile &) = default;
    Tile &operator=(const Tile &) = default;
    Tile(Tile &&) = default;
    Tile &operator=(Tile &&) = default;

    SpriteAssetComponent &getSpriteAsset() { return spriteAsset_; }
    const SpriteAssetComponent &getSpriteAsset() const { return spriteAsset_; }

    SpriteFrameComponent &getSpriteFrame() { return spriteFrame_; }
    const SpriteFrameComponent &getSpriteFrame() const { return spriteFrame_; }

    TerrainType getTerrain() const { return terrain_; }
    void setTerrain(TerrainType terrain) { terrain_ = terrain; }

    MovementMode getMovementFlags() const { return movementFlags_; }
    void setMovementFlags(MovementMode flags) { movementFlags_ = flags; }

    void setSpriteAsset(const SpriteAssetComponent &asset) { spriteAsset_ = asset; }
    void setSpriteFrame(const SpriteFrameComponent &frame) { spriteFrame_ = frame; }

private:
    SpriteAssetComponent spriteAsset_;
    SpriteFrameComponent spriteFrame_;
    TerrainType terrain_ = TerrainType::Ground;          // defaults to walkable
    MovementMode movementFlags_ = MovementMode::Walking; // default preserves original walk-only behavior
};