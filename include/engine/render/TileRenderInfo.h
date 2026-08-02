#pragma once
#include "engine/render/RenderLayer.h"
#include <string>

// Engine-facing view of "what to draw at this tile", populated by the
// game layer from its own SpriteAssetComponent/SpriteFrameComponent.
// RenderSystem needs nothing else about a tile's gameplay representation.
struct TileDrawInfo {
    std::string frameName;
    float renderScale = 1.f;
    RenderLayer layer = RenderLayer::Terrain;
    float z = 0.f;
};