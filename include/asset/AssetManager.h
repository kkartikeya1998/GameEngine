#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "asset/IAssetRepository.h"
#include "asset/TileRepository.h"
#include "asset/MapObjectRepository.h"
#include "asset/SpriteRepository.h"

// ---------------------------------------------------------------------------
// AssetManager — single owner of every metadata repository.
//
// Loads all metadata once at construction (see AssetManager.cpp) and serves
// it out by type at runtime via get<T>(). Repositories are stored behind
// IAssetRepository and keyed by std::type_index, so the set of asset
// categories AssetManager knows about is never hardcoded into its public
// interface — only into its constructor body.
//
// Open/Closed in practice: adding a new asset category (e.g.
// AnimationRepository) means writing that repository class and adding one
// emplace<AnimationRepository>(...) line inside AssetManager's constructor.
// AssetManager.h itself — this file — never needs to change.
//
// AssetManager owns metadata only (parsed JSON describing what assets
// exist and their properties). It deliberately does not own textures,
// sounds, or other backend/GPU resources — those stay a renderer-side
// concern (see SFMLRenderer's atlases), so this class has no dependency
// on any particular rendering backend.
// ---------------------------------------------------------------------------
class AssetManager {
public:
    explicit AssetManager(const std::string& assetsRoot);

    // Retrieves a previously loaded repository by its concrete type.
    // Throws std::runtime_error if T was never registered in the
    // constructor below.
    //
    // Usage: auto& tiles = assetManager.get<TileRepository>();
    template <typename T>
    T& get() const {
        auto it = repositories_.find(std::type_index(typeid(T)));
        if (it == repositories_.end()) {
            throw std::runtime_error(
                std::string("AssetManager: repository not registered: ") + typeid(T).name());
        }
        return static_cast<T&>(*it->second);
    }

private:
    // Constructs a T in place and registers it under its own type.
    // Private: registration only happens here, during construction, so the
    // full set of loaded asset categories is always visible in one place
    // (AssetManager.cpp) rather than scattered across arbitrary call sites.
    template <typename T, typename... Args>
    void emplace(Args&&... args) {
        static_assert(std::is_base_of_v<IAssetRepository, T>,
                      "T must derive from IAssetRepository");
        repositories_[std::type_index(typeid(T))] =
            std::make_unique<T>(std::forward<Args>(args)...);
    }

    std::unordered_map<std::type_index, std::unique_ptr<IAssetRepository>> repositories_;
};