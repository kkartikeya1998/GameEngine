#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "asset/repositories/IAssetRepository.h"
#include "asset/repositories/TileRepository.h"
#include "asset/repositories/MapObjectRepository.h"
#include "asset/repositories/CharacterRepository.h"
#include "asset/AsssetPaths.h"

// ---------------------------------------------------------------------------
// AssetManager — single owner of every metadata repository.
// ---------------------------------------------------------------------------
class AssetManager {
public:
    // All metadata loaded at intialization
    AssetManager() {
        emplace<TileRepository>(Assets::Maps::TILESET_METADATA);
        emplace<MapObjectRepository>(Assets::Maps::OBJECT_METADATA);
        emplace<CharacterRepository>(Assets::Sprites::SPRITE_METADATA);
    }

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