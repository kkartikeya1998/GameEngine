#pragma once
#include <unordered_map>
#include "asset/repositories/IAssetRepository.h"
#include "asset/metadata/CollisionData.h"

template <typename KeyT>
class CollisionAssetRepository : public IAssetRepository {
public:
    void ingest(std::unordered_map<KeyT, CollisionData>&& boxes)
    {
        for (auto& [key, box] : boxes)
            boxes_.insert_or_assign(key, std::move(box));
    }

    // nullptr is the normal, expected case for a tile — not an error
    const CollisionData* find(const KeyT& key) const
    {
        auto it = boxes_.find(key);
        return it == boxes_.end() ? nullptr : &it->second;
    }

private:
    std::unordered_map<KeyT, CollisionData> boxes_;
};