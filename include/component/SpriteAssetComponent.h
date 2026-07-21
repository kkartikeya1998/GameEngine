#pragma once
#include <string>
#include "render/RenderLayer.h"

// Identity + placement metadata for a renderable entity. Set once at
// EntityFactory::populate() time; AnimationSystem never touches this.
struct SpriteAssetComponent {
    std::string assetId;          // permanent metadata lookup id
    float renderScale = 1.f;
    RenderLayer layer = RenderLayer::Terrain;
    float z = 0.f;                // static z — used as-is by tiles; entities override at submit time with position-derived depth

    SpriteAssetComponent() = default;
    SpriteAssetComponent(std::string assetId, RenderLayer layer, float renderScale = 1.f, float z = 0.f)
        : assetId(std::move(assetId)), renderScale(renderScale), layer(layer), z(z) {}
};