#pragma once

#include "component/SpriteAssetComponent.h"
#include "component/SpriteFrameComponent.h"

#pragma once

enum class TerrainType {
    Normal,
    Solid,  // blocks movement — trees, water edges, walls
    Grass   // walkable, triggers wild encounter checks
};

class Tile {
public:
    Tile() = default;
    Tile(const Tile&) = default;
    Tile& operator=(const Tile&) = default;
    Tile(Tile&&) = default;
    Tile& operator=(Tile&&) = default;

    SpriteAssetComponent& getSpriteAsset() { return spriteAsset_; }
    const SpriteAssetComponent& getSpriteAsset() const { return spriteAsset_; }

    SpriteFrameComponent& getSpriteFrame() { return spriteFrame_; }
    const SpriteFrameComponent& getSpriteFrame() const { return spriteFrame_; }

    TerrainType getTerrain() const { return terrain_; }
    void setTerrain(TerrainType terrain) { terrain_ = terrain; }

    void setSpriteAsset(const SpriteAssetComponent& asset) { spriteAsset_ = asset; }
    void setSpriteFrame(const SpriteFrameComponent& frame) { spriteFrame_ = frame; }

private:
    SpriteAssetComponent spriteAsset_;
    SpriteFrameComponent spriteFrame_;
    TerrainType terrain_ = TerrainType::Normal; // defaults to walkable, non-grass
};