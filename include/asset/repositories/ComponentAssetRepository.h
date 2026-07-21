#pragma once
#include <string>
#include <unordered_map>
#include "asset/repositories/IAssetRepository.h"
#include "external/json.hpp"

// One repository type, instantiated per asset-backed component metadata
// struct. Replaces TileRepository/MapObjectRepository/CharacterRepository
// as distinct classes — they were all doing this exact loop, keyed by a
// domain-specific metadata struct.
template <typename TMetadata>
class ComponentAssetRepository : public IAssetRepository
{
public:
    explicit ComponentAssetRepository(const nlohmann::json &section)
    {
        for (auto it = section.begin(); it != section.end(); ++it)
            metadata_.emplace(it.key(), TMetadata::fromJson(it.key(), it.value()));
    }

    const TMetadata *find(const std::string &id) const
    {
        auto it = metadata_.find(id);
        return it == metadata_.end() ? nullptr : &it->second;
    }

    std::string debugString() const
    {
        std::ostringstream oss;
        for (const auto &[id, meta] : metadata_)
        {
            oss << id << " => " << meta << '\n';
        }
        return oss.str();
    }

private:
    std::unordered_map<std::string, TMetadata> metadata_;
};