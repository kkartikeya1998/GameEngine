#include "asset/AssetManager.h"

// ---------------------------------------------------------------------------
// AssetManager's constructor is the one place that lists every asset
// category the game currently loads. Adding a new category means adding
// one emplace<...>(...) line here — see AssetManager.h for why that's the
// only file this touches.
//
// Paths are built from assetsRoot rather than hardcoded absolute paths, so
// the whole asset tree can move by changing one string at the call site
// (see Game's construction of AssetManager).
// ---------------------------------------------------------------------------
AssetManager::AssetManager(const std::string& assetsRoot) {
    emplace<TileRepository>(assetsRoot + "/maps/tileset_metadata.json");
    emplace<MapObjectRepository>(assetsRoot + "/maps/object_metadata.json");
    emplace<SpriteRepository>(assetsRoot + "/sprites/sprite_metadata.json");
}