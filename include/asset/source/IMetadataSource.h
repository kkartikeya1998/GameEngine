#pragma once
#include <filesystem>
#include "external/json.hpp"

// The ONLY format-aware surface in the whole asset system.
// Everything past load() only ever sees nlohmann::json.
class IMetadataSource {
public:
    virtual ~IMetadataSource() = default;
    virtual nlohmann::json load(const std::filesystem::path& path) const = 0;
};