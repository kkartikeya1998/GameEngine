#pragma once
#include <unordered_map>
#include "asset/repositories/IAssetRepository.h"
#include "asset/metadata/RenderData.h"

template <typename KeyT>
class RenderAssetRepository : public IAssetRepository {
public:
    void ingest(std::unordered_map<KeyT, RenderData>&& boxes)
    {
        for (auto& [key, box] : boxes)
            boxes_.insert_or_assign(key, std::move(box));
    }

    const RenderData* find(const KeyT& key) const
    {
        auto it = boxes_.find(key);
        return it == boxes_.end() ? nullptr : &it->second;
    }

private:
    std::unordered_map<KeyT, RenderData> boxes_;
};