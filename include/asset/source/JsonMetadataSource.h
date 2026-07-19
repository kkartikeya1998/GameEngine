#pragma once
#include "asset/source/IMetadataSource.h"

class JsonMetadataSource : public IMetadataSource {
public:
    Result<nlohmann::json, AssetError> load(const std::filesystem::path &path) const override;
};