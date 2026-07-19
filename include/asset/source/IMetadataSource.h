#pragma once
#include <filesystem>
#include "external/json.hpp"
#include "asset/AssetError.h"
#include "exceptions/Result.h"

// The ONLY format-aware surface in the whole asset system.
// Everything past load() only ever sees nlohmann::json.
class IMetadataSource
{
public:
    virtual ~IMetadataSource() = default;
    virtual Result<nlohmann::json, AssetError> load(const std::filesystem::path &path) const = 0;
};
 